#include "opencv2/opencv.hpp"
#include <netdb.h>
#include <unistd.h>
using namespace std;

void error(const string err, int returnVal=0){
    cout<<"\nError: "<<err<<endl;
    if(returnVal) exit(returnVal);
}

bool PRINT = true;
bool DEBUG = false;
void debug(const string &s){
    if(DEBUG) cout<<"\nDEBUG - [   "<<s<<"   ]"<<endl;  
}

const int MAXBUFSIZE = 32 * 1024;   // 32 kB

struct Video{
    unsigned char packetType;
    char name[1024];
    uint8_t fps;
    uint16_t height;
    uint16_t width;
    uint32_t frameCount; 

    Video(const string &s){
        memcpy(this, s.data(), sizeof(Video));
        name[1023]='\0';
    }

    void info(){
        if(!PRINT) return;
        cout<<"\nVideo Metadata:-";
        cout<<"\n   packetType: "<<int(packetType);
        cout<<"\n   name:       "<<name;
        cout<<"\n   fps:        "<<int(fps);
        cout<<"\n   height:     "<<int(height);
        cout<<"\n   width:      "<<int(width);
        cout<<"\n   frameCount: "<<int(frameCount);
        cout<<endl<<endl;
    }
};
struct Frame{
    uint32_t packetType;
    uint32_t index;
    uint32_t dataPacketCount;
    uint32_t size;

    Frame(const string &s){
        memcpy(this, s.data(), sizeof(Frame));
    }

    void info(){
        if(!PRINT) return;
        cout<<"\n   Frame Metadata:-";
        cout<<"\n       packetType:         "<<int(packetType);
        cout<<"\n       index:              "<<int(index);
        cout<<"\n       dataPacketCount:    "<<int(dataPacketCount);
        cout<<"\n       size:               "<<int(size);
        cout<<endl<<endl;
    }
};
struct Data{
    unsigned char packetType;
    uint32_t frameIndex;
    uint16_t index;
    uint16_t length;
    char data[MAXBUFSIZE - (sizeof(packetType)+sizeof(frameIndex)+sizeof(index)+sizeof(length))];

    Data(const string &s){
        memcpy(this, s.data(), sizeof(Data));
    }
};

int sockfd;     // Common Socket for all communications

void Send(const string &message, struct addrinfo *destination){    // Send a message to given destination
    int len = sendto(sockfd, message.c_str(), message.size(), 0, destination->ai_addr, destination->ai_addrlen);
    if(len<0) error("Sending \"start\" to server", 3);
}

pair<string, struct sockaddr> Receive(){    // Receive message from anyone who sends
    char buf[MAXBUFSIZE];
    sockaddr sender;
    socklen_t addr_len = sizeof(sender);
    int len = recvfrom(sockfd, buf, MAXBUFSIZE, 0, &sender, &addr_len);
    if(len<0){
        if(errno == EAGAIN || errno == EWOULDBLOCK) return {"", sender};   // indicate no data received
        else error("Receiving Video Metadata from Server", 4);
    }
    
    return make_pair(string(buf,len), sender);
}

struct addrinfo* connect_to_server(){
    string server_ip, port; 
    cout<<"\nEnter Server IP: ";
    cin>>server_ip;
    cout<<"Enter Server port: ";
    cin>>port;

    addrinfo hints{}, *server;
    hints.ai_family = AF_INET;        // IPv4 only
    hints.ai_socktype = SOCK_DGRAM;   // UDP

    int status = getaddrinfo(server_ip.c_str(), port.c_str(), &hints, &server);
    if(status) error("getaddrinfo(): " + string(gai_strerror(status)), 1);

    return server;
}

int main(int argc, char* argv[]){
    cout<<endl;
    if(argc>1){
        if(argv[1][0]=='0') PRINT = false;  // print nothing on terminal improves performance
        else DEBUG = true;    // debug() enabled if any non-zero compile time argument is given.
    }
    
    // creating socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd<0) error("Creating Socket", 2);
    cout<<"Socket Created"<<endl;

    // increasing network buffer size
    int bufferSize = MAXBUFSIZE;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));
    cout<<"\nBuffer size increased"<<endl;

    struct timeval tv;
    tv.tv_usec = 100000;        // microseconds (100 ms)
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    addrinfo *server = connect_to_server();
    cout<<"\nConnected to server"<<endl;

    // send message
    Send("START", server);
    cout<<"\nStreaming initialized"<<endl;
    
    // recv message
    auto [video_metadata, sender] = Receive();
    debug("video_metadata = " + video_metadata);
    debug("Length of Video Metadata = " + to_string(video_metadata.size()));
    Video video(video_metadata);
    video.info();

    for(int i=0;i<video.frameCount;i++){
        if(PRINT){
            cout<<"Frame "<<i<<": ";
            fflush(stdout);
        }
        string request = "REQ xxxx";
        memcpy(request.data() + 4, &i, 4);
        Send(request, server);

        auto [frame_metadata, sender] = Receive();
        if(frame_metadata.empty()){
            if(PRINT) cout<<"Time out receiving Frame Metadata"<<endl;
            i--;
            continue;
        }
        Frame framed(frame_metadata);
        // framed.info();
        // debug("frame_metadata = " + frame_metadata);
        // debug("Length of Frame Metadata = " + to_string(frame_metadata.size()));

        if(framed.index != i){      // Frame index check
            if(PRINT) cout<<"Incorrect Frame index = "<<framed.index<<" received"<<endl;
            i--;
            continue;
        }

        vector<uchar> encodedFrame;
        bool correct = true;
        int total_size = 0;
        for(int d=0;d<framed.dataPacketCount;d++){
            auto [frame_data_packet, sender] = Receive(); 
            if(frame_data_packet.empty()){
                if(PRINT) cout<<"Timeout receiving Frame Data Packet"<<endl;
                correct = false;
                break;
            }
            Data packet(frame_data_packet);
            // debug("frame_data_packet = " + frame_data_packet);
            // debug("Length of Frame Data Packet = " + to_string(frame_data_packet.size()));
            if((packet.frameIndex != i) || (packet.index != d)){
                if(PRINT){
                    printf("frameIndex %d instead of %d. dataPacket index %d instead of %d received",packet.frameIndex,i,packet.index,d);
                    cout<<endl;
                }
                correct = false;
                break;
            }
            total_size += packet.length;
            if(total_size > framed.size){
                if(PRINT){
                    printf("Total frame size = %d exceeded expected frame size = %d",total_size,framed.size);
                    cout<<endl;
                }
                correct = false;
                break;
            }
            for(int p=0;p<packet.length;p++) encodedFrame.push_back(packet.data[p]);
        }
        
        if(!correct){
            i--;
            continue;
        }
        else if(total_size != framed.size){
            if(PRINT) cout<<"Incomplete Frame. Expected size = "<<framed.size<<". Received = "<<encodedFrame.size()<<endl;
            i--; // request frame again
            continue;
        }

        if(PRINT) cout<<"Correctly Received"<<endl;

        // Decoding
        cv::Mat frame;
        cv::imdecode(encodedFrame, cv::IMREAD_COLOR, &frame);
    
        // If the frame is empty, break immediately
        if (frame.empty()) break;
    
        // Display the resulting frame
        cv::imshow( "Frame", frame );

        // Press  ESC on keyboard to exit
        char c=(char) cv::waitKey(25);
        if(c==27) break;
    }
 
    // Closes all the frames
    cv::destroyAllWindows();

    close(sockfd);

    cout<<endl;
    return 0;
}