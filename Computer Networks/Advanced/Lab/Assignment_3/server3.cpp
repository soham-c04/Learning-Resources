#include <iostream>
#include <netdb.h>
#include <map>
#include <cstring>
#include <unistd.h>
using namespace std;

/* NOTE:- This code works only with the linux game client - `./clients/client/client` and not with `clients/client.exe` */

void error(const string err, int returnVal=0){
    cout<<"\nError: "<<err<<endl;
    if(returnVal) exit(returnVal);
}

bool PRINT = true;
bool DEBUG = false;
void debug(const string &s){
    if(DEBUG) cout<<"\nDEBUG - [   "<<s<<"   ]"<<endl;  
}

const int MAXBUFSIZE = 1024;

/* ----- Game Data ----- */

#pragma pack(push, 1)

enum Type : uint8_t {
    INVALID = 0,
    CONNECT = 1,
    PLAYER_ID = 1,
    PLAYER_ID_ACK = 2,
    SPAWN_PLAYER = 2,
    CONN_CLOSE = 3,
    DESPAWN_PLAYER = 3,
    PLAYER_INPUT = 4,
    PLAYER_MOVE = 4,
    PLAYER_POS = 5,
    PLAYER_POS_S = 5
};

struct Connect{       // Used for :- Connect
    Type packetType;
    double x;
    double y;

    Connect(const string &s){
        if(s.size() != sizeof(Connect)){
            debug("Connect:: packet_size = " + to_string(s.size()) + "  Connect size = " + to_string(sizeof(Connect)));
            packetType = INVALID;
        }
        else memcpy(this, s.data(), sizeof(Connect));
    }

    const string convert(){
        string s;
        s.resize(sizeof(Connect));
        memcpy(s.data(), this, sizeof(Connect));
        return s;
    }

    void info(){
        if(!PRINT) return;
        cout<<"\nConnection:-";
        cout<<"\n   packetType: "<<int(packetType);
        cout<<"\n   (x,y):      ("<<x<<","<<y<<")";
        cout<<endl<<endl;
    }
};
struct ID{          // Used for :- Connection Close, Player ID and Despawn Player
    Type packetType;
    uint8_t player_id;

    ID(const string &s){
        if(s.size() != sizeof(ID)) packetType = INVALID;
        else memcpy(this, s.data(), sizeof(ID));
    }

    ID(Type pType, uint8_t id){
        packetType = pType;
        player_id = id;
    }

    const string convert(){
        string s;
        s.resize(sizeof(ID));
        memcpy(s.data(), this, sizeof(ID));
        return s;
    }

    void info(){
        if(!PRINT) return;
        cout<<"\nPlayer ID:-";
        cout<<"\n   packetType: "<<int(packetType);
        cout<<"\n   player_id:  "<<int(player_id);
        cout<<endl<<endl;
    }
};
struct Input{       // Used for :- Player Input and Move
    Type packetType;
    uint8_t player_id;
    double x;
    double y;
    uint8_t dir;
    uint8_t jumped;

    Input(const string &s){
        if(s.size() != sizeof(Input)) packetType = INVALID;
        else memcpy(this, s.data(), sizeof(Input));
    }

    const string convert(){
        string s;
        s.resize(sizeof(Input));
        memcpy(s.data(), this, sizeof(Input));
        return s;
    }

    void info(){
        if(!PRINT) return;
        cout<<"\n   Player Input:-";
        cout<<"\n       packetType: "<<int(packetType);
        cout<<"\n       player_id:  "<<int(player_id);
        cout<<"\n       (x,y):      ("<<x<<","<<y<<")";
        cout<<"\n       dir:        "<<int(dir);
        cout<<"\n       jumped:     "<<int(jumped);
        cout<<endl<<endl;
    }
};
struct Pos{         // Used for :- Player_ID_ACK, Player_Pos, Spawn Player and Player_Pos_S
    Type packetType;
    uint8_t player_id;
    double x;
    double y;

    Pos(const string &s){
        if(s.size() != sizeof(Pos)) packetType = INVALID;
        else memcpy(this, s.data(), sizeof(Pos));
    }

    Pos(Type pType, uint8_t id, double X, double Y){
        packetType = pType;
        player_id = id;
        x = X;
        y = Y;
    }

    const string convert(){
        string s;
        s.resize(sizeof(Pos));
        memcpy(s.data(), this, sizeof(Pos));
        return s;
    }

    void info(){
        if(!PRINT) return;
        cout<<"\n   Player Position:-";
        cout<<"\n       packetType: "<<int(packetType);
        cout<<"\n       player_id:  "<<int(player_id);
        cout<<"\n       (x,y):      ("<<x<<","<<y<<")";
        cout<<endl<<endl;
    }
};

#pragma pack(pop)

/* ------- UDP COnnection ------ */

int sockfd;     // Common Socket for all communications

tuple<string, struct sockaddr_storage, socklen_t> Receive(){    // Receive message from anyone who sends
    char buf[MAXBUFSIZE];
    sockaddr_storage sender{};
    socklen_t addr_len = sizeof(sender);
    int len = recvfrom(sockfd, buf, MAXBUFSIZE, 0, (struct sockaddr*)&sender, &addr_len);
    if(len<0){
        if(errno == EAGAIN || errno == EWOULDBLOCK) return {"", sender, addr_len};   // indicate no data received
        else error("Receiving from Client", 5);
    }
    
    return {string(buf,len), sender, addr_len};
}

void Send(const string &message, const sockaddr_storage &destination, socklen_t addr_len){    // Send a message to given destination
    int len = sendto(sockfd, message.data(), message.size(), 0, (const sockaddr*)&destination, addr_len);
    if(len<0) error("Sending to Client", 4);
}


int start_server(){
    string port;
    cout<<"Enter port to start server: ";
    cin>>port;

    addrinfo hints{}, *res;

    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_DGRAM;   // UDP
    hints.ai_flags = AI_PASSIVE;      // Accepts packets also from non-loopback interfaces also, e.g:- WiFi or LAN.

    int status = getaddrinfo(NULL, port.c_str(), &hints, &res);
    if(status) error("getaddrinfo(): " + string(gai_strerror(status)), 1);

    // Create socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sockfd<0) error("Creating Socket", 2);

    // Bind socket
    if(bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) error("Binding to Socket", 3);

    cout<<"\nUDP Server started on port: "<<port<<endl;

    return sockfd;
}

/* ----- Handle Client Message ----- */

struct Client{
    uint8_t player_id;
    double x,y;
    sockaddr_storage address;
    socklen_t addr_len;

    Client(){};

    Client(uint8_t id, Pos &ack, sockaddr_storage &addr, socklen_t len){
        player_id = id;
        x = ack.x;
        y = ack.y;
        address = addr;
        addr_len = len;
    }
};

uint8_t players = 0;
map<uint8_t, Client> active_players; 

void Connection(Connect &connect, sockaddr_storage &client, socklen_t addr_len){
    players++;
    ID id(PLAYER_ID, players);              // Generate next_player_id
    Send(id.convert(), client, addr_len);   // Sending player_id back to client sending CONNECT request.
}

void Player_ID_ACK(Pos &ack, sockaddr_storage &client, socklen_t addr_len){
    // PLAYER_ID_ACK doesn't send and player_id with it. How to extract "player_id" from it ? It only sends double x,y (Same as Connect)

    uint8_t player_id = ack.player_id;                                  // Extracting player_id from ACK.

    if(active_players.find(player_id) != active_players.end()){     // Client with given player_id already previously present.
        cout<<"\nClient with player_id = "<<player_id<<" exists previously"<<endl;        
        return;     
    }

    Client new_client(player_id, ack, client, addr_len);

    cout<<"\nNew Connection: Client with player_id = "<<player_id<<"    spawned at location :- ("<<new_client.x<<","<<new_client.y<<")"<<endl;

    Pos new_spawn(SPAWN_PLAYER, player_id, new_client.x, new_client.y);
    string message = new_spawn.convert();

    for(auto [id, player]:active_players){
        Send(message, player.address, player.addr_len);             // Sending all older clients the spawn location of new_client.
        Pos old_spawn(SPAWN_PLAYER, player.player_id, player.x, player.y);
        Send(old_spawn.convert(), client, addr_len);                // Sending new_client locations of all older clients. 
    }

    active_players[player_id] = new_client;                         // Add the new_client to the active_players hashmap.
}

void Connection_Close(ID &close){
    if(active_players.find(close.player_id) == active_players.end()) return;    // player_id not found active_players list

    active_players.erase(active_players.find(close.player_id));                 // Remove player_id from active_players list.

    ID despawn(DESPAWN_PLAYER, close.player_id);
    string message = despawn.convert();

    for(auto [id,player]:active_players){
        Send(message, player.address, player.addr_len);                         // Sending DESPAWN_PLAYER to all remaining clients. 
    }
}

void Player_Input(Input &input){
    uint8_t player_id = input.player_id;
    if(active_players.find(player_id) == active_players.end()) return;    // player_id not found active_players list
    
    active_players[player_id].x = input.x;
    active_players[player_id].y = input.y;
    Input move(input);
    move.packetType = PLAYER_MOVE;
    string message = move.convert();
    for(auto [id,player]:active_players){
        Send(message, player.address, player.addr_len);
    }
}

void Player_Position(Pos &pos){
    uint8_t player_id = pos.player_id;
    if(active_players.find(player_id) == active_players.end()) return;      // player_id not found active_players list

    active_players[player_id].x = pos.x;
    active_players[player_id].y = pos.y;
    Pos forward(pos);
    forward.packetType = PLAYER_POS_S;
    string message = forward.convert();
    for(auto [id,player]:active_players){
        Send(message, player.address, player.addr_len);
    }
}

int main(int argc, char* argv[]){
    cout<<endl;
    if(argc>1){
        if(argv[1][0]=='0') PRINT = false;  // print nothing on terminal. Improves performance.
        else DEBUG = true;    // debug() enabled if any non-zero compile time argument is given.
    }
    
    sockfd = start_server();

    while(true){
        auto [packet, client, addr_len] = Receive();
        debug("packet_size = " + to_string(packet.size()));
        
        Input input(packet);
        if(input.packetType == PLAYER_INPUT){
            Player_Input(input);
            continue;
        }

        Pos pos(packet);
        if(pos.packetType == PLAYER_POS){
            Player_Position(pos);
            continue;
        }
        else if(pos.packetType == PLAYER_ID_ACK){
            Player_ID_ACK(pos, client, addr_len);
            continue;
        }

        Connect connect(packet);
        if(DEBUG) connect.info();
        if(connect.packetType == CONNECT){
            Connection(connect, client, addr_len);
            continue;
        }

        ID id(packet);
        if(id.packetType == CONN_CLOSE){
            Connection_Close(id);
            continue;
        }

        if(PRINT) error("Invalid Packet Type/Format");
        
    }

    close(sockfd);

    cout<<endl;
    return 0;
}