#include <cstdlib>
#include <../include/connectionHandler.h>
#include <../include/Books.h>
#include <../include/UserIO.h>
#include <../include/SocketIO.h>
#include <thread>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>



/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/


int main (int argc, char *argv[]) {

    //defining mutexes for books
    std::mutex mutex;
    std::mutex mutex_Receipt;
    std::mutex mutex_id;
    Books books(mutex,mutex_Receipt,mutex_id);

    ConnectionHandler *connectionHandler = nullptr;

    boost::atomic_bool *connected=new boost::atomic_bool(false);



    std::string username;
    std::string password;
    std::string HostnPort;


    const short bufsize = 1024;
    char buf[bufsize];
    std::string line;


    while (!connected->load())
    {


        //login input
        if (line.empty()){
            do
                {
                std::cout<< "Enter command: login host:port user-name password"<<std::endl;
                std::cin.getline(buf, bufsize);
                std::string line2(buf);
                line = std::move(line2);

                }
            while (line.substr(0, 5) != "login"&&line.substr(0,3)!="bye");
        }

        if(line.substr(0,3)=="bye")
        {
            break;
        }



        int pos = line.find(' ',6);
        HostnPort = line.substr(6,pos-6);

        int pos2 = line.find(' ',pos+1 );
        username = line.substr(pos+1,pos2-pos-1);
        password = line.substr(pos2+1);



        //Host
    std::string host = HostnPort.substr(0,HostnPort.find(':'));
        //Port
    std::string portStr= HostnPort.substr(HostnPort.find(':')+1);
    short port = atoi(portStr.c_str());


    connectionHandler= new ConnectionHandler(host, port);


        if (!connectionHandler->connect()) {
            line.clear();
            std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        }

        else{

            *connected=true;
        std::string connectedFrame="CONNECT\naccept-version:1.2\nhost:"+HostnPort+"\nlogin:"+username+"\npasscode:"+password+"\n\n\0";
        std::cout<<connectedFrame<<std::endl;
        connectionHandler->sendFrameAscii(connectedFrame,'\0');
            std::string answer;
            // Get back an answer: by using the expected number of bytes (len bytes + newline delimiter)
            // We could also use: connectionHandler.getline(answer) and then get the answer without the newline char at the end
            if (!connectionHandler->getFrameAscii(answer,'\0'))
            {
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }

            answer.resize(answer.length()-1);
            if (answer.find("CONNECTED")!=std::string::npos)
            {
                if (answer.find("version:1.2"))
                {
                    std::cout<<"Connected and running"<<std::endl;

                    UserIO userIo(books,username,*connectionHandler,connected);
                    SocketIO socketIo(username,*connectionHandler,books,connected);

                    //socketIOThread runs on a different thread
                    std::thread socketIoThread(&SocketIO::run, &socketIo);
                    //UserIO runs on main thread
                    userIo.run(line);

                    socketIoThread.join();


                }

                else{

                    std::cout<<"version not compatible"<<std::endl;
                    }

            }

            else if(answer.find("ERROR"))
            {
                std::cout<<answer<<std::endl;
            }

            else
                {
                std::cout<<"didn't get CONNECTED in frame"<<std::endl;
            }
        }
    }

    delete connectionHandler;
    delete connected;


    return 0;
}
