#include <bits/stdc++.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <regex.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;

void error(const string err, bool close=false){
    cout<<"\nError: "<<err<<endl;
    if(close) exit(1);
}
 
bool DEBUG = false;
void debug(const string s){
    if(DEBUG){
        cout<<"\nDEBUG - [   "<<s<<"   ]"<<endl;  
        cout<<"\nYou: ";
        fflush(stdout);
    }
}

const int STDIN = 0;            // Non-blocking user-input via keyboard.
const int BUFFER = 32768;
const int TTL = 10;

int my_fd, max_fd;
fd_set all_neighbors;           // Set of all peers with whom I am directly connected to.
vector<int> neighbor_fds;       // Better Efficiency - while iterating through FD_ISSET().
set<int> seen_messages;         // To prevent forever looping of a message (Temporary cache)
int peers=1;                    // Total number of active peers

const int MOD=1e9+7, base='z'+5;
int Hash(const string &name, int n=peers){   // Hash function for DHT (Horner's Method)
    int hashval=0;
    for(char c:name) hashval = (hashval*1LL*base+c)%MOD;
    return hashval%n;
}

struct Peer{
    int id;
    string name;
    string ip;
    int port;
    int sockfd;                 // If Peer is a neighbor then what is his sockfd. Set to -1 if Peer is not a neighbor.
    bool reachable;             // Has to be set to "false" if after disconnection of a certain client this Peer is no longer reachable.

    Peer(int id=-1, string name="", string ip="", int port=-10, int sockfd=-1){
        this->id = id;
        this->name = name;
        this->ip = ip;
        this->port = port;
        this->sockfd = sockfd;
        reachable = true;
    }

    bool operator==(const Peer& other) const{
        return(ip==other.ip) && (port==other.port);
    }

    void display(int padding){
        if(id==0){
            cout<<"(You) ";
            padding-=6;
        }
        printf("%-*s : ID=%d    IP=%s   port=%d\n", padding, name.c_str(), id, ip.c_str(), port);
    }

    void print(string message, bool private_=false){
        if(private_) printf("\n[%d] %s -> You : %s\n", id, name.c_str(), message.c_str());
        else printf("\n[%d] %s : %s\n", id, name.c_str(), message.c_str());
        fflush(stdout);
    }

    string encode(){
        return ip + "#" + to_string(port) + "#" + name + "\n"; 
    }

    static tuple<string, int, string> decode(string &s){
        stringstream ss(s);
        string ip, port, name;
        if(!getline(ss, ip, '#')) return {"", -10, ""};
        if(!getline(ss, port, '#')) return {"", -10, ""};
        if(!getline(ss, name, '\n')) return {"", -10, ""};
        int portno;
        try{
            portno = stoi(port);
        }
        catch(const invalid_argument &e){
            error("Peer::decode - Port no should be an integer - " + port);
        }
        return {ip, portno, name};
    }

    void connected(){
        printf("\n[+] %s Connected : ID=%d    IP=%s port=%d\n", name.c_str(), id, ip.c_str(), port);
        fflush(stdout);
    }

    void disconnected(){
        printf("\n[-] %s(%d) Disconnected\n", name.c_str(), id);
        fflush(stdout);    
        // Reset Peer values   
        id = sockfd = -1;
        port = -10;
        reachable = false;
        name = ip = "";
    }

    int hash(int n=peers){ return Hash(ip + ':' + to_string(port), n); }
};
vector<Peer> peer;              // Set of all Peers (Irrespective of whether I am directly connected to them or not).
map<pair<string,int>, int> ID;  // Maps <ip, port> pair to uniqueID of Peer (different from each Peer POV). Generates a new ID for each new Peer
#define my peer[0]              // My details
#define my_path ("myfiles-" + my.ip + "-" + to_string(my.port))     // Folder where I'm storing my files
const string PATH = "Functions";// Default path where Local files are stored

struct Message{
    int id;
    Peer source;
    Peer destination;
    int ttl;
    int type;
    string payload;
    
    Message(string &s){                 // Decode input string into parts
        if(s.empty()){
            id = 0;
            seen_messages.insert(0);
            debug("Message - Empty String");
            return;
        }
        stringstream ss(s);
        string ID, ttl1, sp, dp, tp;
        getline(ss, ID, '|');
        getline(ss, source.ip, '|');
        getline(ss, sp, '|');
        getline(ss, destination.ip, '|');
        getline(ss, dp, '|');
        getline(ss, ttl1, '|');
        getline(ss, tp, '|');
        getline(ss, payload, '\0');

        // debug("Message - " + ID + " , " + source.ip + " , " + sp + " , " + destination.ip + " , " + dp + " , " + ttl1 + " , " + tp + " , " + payload);  

        try{
            id = stoi(ID);
        }
        catch(const invalid_argument &e){
            error("Message_id should be integer - " + ID, true);
        }
        try{
            ttl = stoi(ttl1);
        }
        catch(const invalid_argument &e){
            error("TTL should be an integer - " + ttl, true);
        }
        try{
            source.port = stoi(sp);
        }
        catch(const invalid_argument &e){
            error("Source port should be an integer - " + sp);
        }
        try{
            destination.port = stoi(dp);
        }
        catch(const invalid_argument &e){
            error("Desination port should be an integer - " + dp);
        }
        try{
            type = stoi(tp);
        }
        catch(const invalid_argument &e){
            error("Message Type should be an integer - " + dp);
        }
    }

    Message(string &payload, int type, int i=-1){    // Message is created for the first time
        id = (int32_t)((uint32_t)rand() | ((uint32_t)rand() << 15) | ((uint32_t)rand() << 30) | my.port);
        source = my;
        if(type>=0) destination = peer[i];  // Private msg
        else{                               // Broadcast
            destination.ip.clear();
            destination.port = 0;
        }
        ttl = TTL;
        this->type = type;
        this->payload = payload;

        seen_messages.insert(id);
    }

    string encode(){                    // Encode Message back into string
        string encoded = to_string(id) + "|" + source.ip + "|" + to_string(source.port) + "|" + destination.ip + "|" + to_string(destination.port) + "|" + to_string(ttl) + "|" + to_string(type) + "|" + payload;
        return encoded;
    }

    bool isSearch(){ return type == -4; }              // O(n^2) - More optimal possible ?
    bool isNewDisconnection(){ return type == -3; }
    bool isNewConnection(){ return type == -2; }
    bool isBroadcast(){ return type==-1; }
    bool isPrivate(){ return type >= 0; }
    bool isSearchReply(){ return type == 1; }
    bool isRequest(){ return type == 2; }
    bool isSendFile(){ return type == 3; }
};

struct File{
    string filename;

    bool operator==(const string& s) const {
        return filename == s;
    }

    static int checkAccess(const string &name);

    string open(string path = my_path){
        ifstream in(path + "/" + filename + ".txt");
        if(!in){
            error("Opening file - " + path + "/" + filename + ".txt");
            return "";
        }
        ostringstream ss;
        ss << in.rdbuf();
        string contents = ss.str();
        return contents;
    }

    File(string &name, bool owner=true){                    // You are the first owner of file OR part of allFiles
        filename = name;
        if(!owner) return;
        ofstream out(my_path + "/" + filename + ".txt");    // Copy to myfiles
        string contents = open(PATH);
        if(out && (!contents.empty())) out << contents;
        else error("Copying file to myfiles");
    }

    File(string &name, string &contents){                   // You are creating a file after copying from someone else.
        filename = name;
        ofstream out(my_path + "/" + filename + ".txt");    // Copy to myfiles
        if(out) out << contents;
        else error("Copying file to myfiles");
    }

    void Send(string &id);

    void Remove(){
        if(remove((my_path + "/" + filename + ".txt").c_str())) error("Removing file from myfiles");
    }
};
vector<File> allFiles, myFiles;                                    // List of all files I have.

int File::checkAccess(const string &name){
    if(find(myFiles.begin(),myFiles.end(), name) != myFiles.end()){
        cout<<"\nYou already have the file"<<endl;
        return 1;                                       // I already have the file
    }
    if(find(allFiles.begin(), allFiles.end(), name)!=allFiles.end()){
        cout<<"\nThis is a private file.\nUse /request <filename> to request for file access from owner"<<endl;
        return 0;                                           // Someone has the file. You need permission.
    }
    else{
        cout<<"\nFile not Found"<<endl;
        return -1;                                          // File not Found
    }
}

void getallFiles(){
    DIR *dir = opendir(PATH.c_str());
    if (!dir) {
        error("Directory - " + PATH + " doesn't exist");
        return;
    }

    struct dirent *entry;
    allFiles.clear();
    while((entry = readdir(dir)) != nullptr){
        if (entry->d_type != DT_REG) continue;      // Don't consider if not a regular file

        string filename = entry->d_name;
        if (filename.size() < 5 || filename.substr(filename.size() - 4) != ".txt") continue;    // Consider only .txt files
        for(int i=0;i<4;i++) filename.pop_back();
        allFiles.push_back(File(filename, false));
    }

    closedir(dir);
}

void updatemyFiles(int prev_n){  // Only keep previously received files from someone else
    for (auto it = myFiles.begin(); it != myFiles.end();) {        // Remove previous default files
        if(Hash(it->filename, prev_n) == my.hash(prev_n)) {
            it->Remove();
            myFiles.erase(it);
        }
        else ++it;
    }
    for(File &f:allFiles){
        if(Hash(f.filename)==my.hash()) myFiles.push_back(File(f.filename));    // Add new default files
    }
}

bool match(const string &text, string &pattern) {           // Regex Matching code
    if(pattern.empty()) pattern = ".*";
    regex_t re;
    if (regcomp(&re, pattern.c_str(), REG_EXTENDED | REG_NOSUB) != 0) return false;

    int result = regexec(&re, text.c_str(), 0, NULL, 0);
    regfree(&re);
    return result == 0;
}

void add_neighbor(int sockfd){          // New Peer added to set of neighbors
    FD_SET(sockfd, &all_neighbors);
    neighbor_fds.push_back(sockfd);
    max_fd = max(max_fd, sockfd);
}

void remove_neighbor(int sockfd){       // Don't remove Peer with sockfd from ID[] or peer[] list. In that case both ID[] and Peer[] for all Peers having id>ID[sockfd] will need to be updated. We have to use set<int> neighbor_fds to efficiently support that in O(logn).
    close(sockfd);
    FD_CLR(sockfd, &all_neighbors);
    while(!FD_ISSET(max_fd, &all_neighbors)) max_fd--;
    neighbor_fds.erase(find(neighbor_fds.begin(), neighbor_fds.end(), sockfd));
}

string Read(int client_fd){             // Reading from a Single Specified Peer(neighbor), given the client_fd.
    char data[BUFFER+1];
    memset(data, 0, sizeof(data));
    int n = read(client_fd, data, sizeof(data));
    if(n<=0){
        error("Improper Read - " + to_string(n));
        return "";
    }
    data[n]='\0';
    // debug("Read: " + string(data) + " ("+to_string(client_fd)+")");
    return string(data);
}

bool Send(int sockfd,const string &message){                // Sending to a Single Specified Peer (neighbor), given the sockfd.
    ssize_t n = write(sockfd, message.c_str(), message.size()); 
    if(n < 0){
        error("Writing to socket - " + to_string(sockfd));
        debug("Return value - " + to_string(n));
        return false;
    }
    // debug("Write: " + message + " ("+to_string(sockfd)+")");
    sleep(1);
    return true;
}

void forward(Message &msg, int parent=-1){                  // Compelete flooding to reach destination
    string message = msg.encode();
    // Check if destination is a neighbor
    for(Peer &p:peer){
        if((p.sockfd!=-1) && (p.sockfd!=parent) && (msg.destination == p)){       // Condition for neighbor is the destination
            if(Send(p.sockfd, message)) return;
        }
    }

    for(int fd:neighbor_fds){
        if(fd!=parent){
            Send(fd, message);
        }
    }
}

void help(){
    cout<<"\nAvailable commands:"<<endl;
    // Assignment 2 Part 1 - P2P Chat Group
    cout<<" /help                       - Show this message"<<endl;
    cout<<" /list                       - List connected peers"<<endl;
    cout<<" /list neighbors             - List of neighbors"<<endl;
    cout<<" /connect <IP> <port>        - Connect to a peer"<<endl;
    cout<<" /msg <ID> <message>         - Send private message to peer"<<endl;
    cout<<" /broadcast <message>        - Broadcast message to all peers"<<endl;
    // Assignment 2 Part 2 - P2P File Sharing
    cout<<" /allfiles [regex]           - List all LOCAL files in \"Functions/\" and satisfying regex"<<endl;
    cout<<" /removefile <filename>      - Remove a file"<<endl;
    cout<<" /myfiles [regex]            - Show all myFiles satisfying regex"<<endl;
    cout<<" /readfile <filename>        - Read a file. Can be opened only if you have access."<<endl;
    cout<<" /search [regex]             - Search for all GLOBAL files (which belong to someone) satisfying regex"<<endl;
    cout<<" /request <filename>         - Request file from someone"<<endl;
    cout<<" /sendfile <ID> <filename>   - Send file to someone"<<endl;
    cout<<" /exit                       - Show this message"<<endl;
    cout<<" /broadcast is done by default is no command is mentioned"<<endl;
    cout<<"\nTo send a public message, just type and press enter"<<endl;
    cout<<"Important:- There is a 1 second timeout before each Send()"<<endl;
    cout<<"Only *.txt files will be considered"<<endl;
}

void List(bool neighbors_only){
    int max_len = my.name.size()+6;
    for(Peer &p:peer) max_len = max(max_len, (int)p.name.size());
    bool printed = false;
    for(Peer &p: peer){
        if(!p.reachable){
            continue;
        }
        else if(neighbors_only){
            if(p.sockfd != -1){
                printed = true;
                p.display(max_len);
            }
        }
        else p.display(max_len);
    }
    if(neighbors_only && (!printed)) cout<<"\n- No neighbors to Display";
    cout<<endl;
}

void msg(string &payload, int type, const string &destination_ID="-1"){    // Flooding when ID is given
    int destination_id;
    try{
        destination_id = stoi(destination_ID);
    }
    catch(const invalid_argument &e){
        error("Peer ID should be an integer - " + destination_ID);
        return;
    }
    if(destination_id >= (int)peer.size()){
        error("Invalid Destination id (out of bounds) - " + to_string(destination_id));
        debug("ID = " + destination_ID + "  >=  #peers = " + to_string(peer.size()));
        return;
    }

    Message message(payload, type, destination_id);
    forward(message);
}

void castNewConnection(int sockfd){
    string payload;
    for(Peer &p:peer) payload += p.encode();
    Message announce(payload, -2);
    string message = announce.encode();
    Send(sockfd, message);
}

void readNewConnection(Message &req){
    stringstream ss(req.payload);
    while(true){
        string ip, port, name;
        if(!getline(ss, ip, '#')) break;
        if(!getline(ss, port, '#')) break;
        if(!getline(ss, name, '\n')) break;
        // debug("New Connection - " + ip + " , " + port + " , " + name);
        int portno;
        try{
            portno = stoi(port);
        }
        catch(const invalid_argument &e){
            error("New Connection port should be an integer - " + port);
            debug(req.payload + " - " + ip + " , " + port + " , " + name);
        }
        auto it = ID.find({ip, portno});
        int id;
        if(it == ID.end()){                                     // Already connected previously
            id = ID[{ip, portno}] = ID.size();
            peer.push_back(Peer(id, name, ip, portno));
            peer[id].connected();
        }
        else if(it->second > 0){
            id = it->second;
            peer[id].name = name;
        }
    }
}

void Connect(string &IP, string &serv_port){
    int portno;
    try{
        portno = stoi(serv_port);
    }
    catch(const invalid_argument &e){
        error("Port no. should be an integer - " + serv_port);
        return;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("Socket - " + to_string(sockfd));
        return;
    }
    
    // Check if peer is already a neighbor. In that case don't connect again
    for(Peer &p:peer){
        if(p.sockfd!=-1){
            if((p.ip==IP) && (p.port==portno)) return;
        }
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);

    if(inet_pton(AF_INET, IP.c_str(), &serv_addr.sin_addr) <= 0){
        error("Invalid IP - " + IP);
        close(sockfd);
        return;
    }
    else if(connect(sockfd, (sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        error("Connect - " + serv_port);
        close(sockfd);
        return;
    }

    add_neighbor(sockfd);
    Send(sockfd, my.encode());
    sleep(1);
    string name = Read(sockfd);
    // debug("Listen - " + name);
    auto it = ID.find({IP, portno});
    if(it == ID.end()){                                     
        int id = ID[{IP, portno}] = ID.size();
        peer.push_back(Peer(id, name, IP, portno, sockfd));
        peer.back().connected();
        castNewConnection(sockfd);     // Cast Connection only if they were not previously connected.
        // debug("Accept casted");
    }
    else{
        peer[it->second].sockfd = sockfd;
        debug("Preivously Connected");
    }
}

void Accept(){
    int client_fd = accept(my_fd, NULL, NULL);
    if(client_fd < 0) error("Accept - " + to_string(client_fd));
    else{
        add_neighbor(client_fd);
        string s = Read(client_fd);
        sleep(1);
        Send(client_fd, my.name);
        // debug("Accept - " + s);
        auto [ip, portno, name] = Peer::decode(s);
        auto it = ID.find({ip, portno});
        if(it == ID.end()){                                     
            int id = ID[{ip, portno}] = ID.size();
            peer.push_back(Peer(id, name, ip, portno, client_fd));
            peer.back().connected();
            castNewConnection(client_fd);
        }
        else{
            peer[it->second].name = name;
            peer[it->second].sockfd = client_fd;
            debug("Previously Connected");
        }
    }
}

void castNewDisconnection(){
    string payload = "";
    msg(payload, -3);
}

// How to handle deletions at articulations points ?
// /sync_state

bool readNewDisconnection(Message &req, int client_fd){     // Don't do ID.erase() or peers.erase().
    auto it = ID.find({req.source.ip, req.source.port});
    if(it == ID.end()) return true;
    int id = it->second;
    if((peer[id].sockfd == client_fd) && (peer[id] == req.source)){     // Peer who disconnected was a neighbor to me.
        peer[id].disconnected();
        return false;
    }
    peer[id].disconnected();
    return true;
}

void readSearch(Message &req){
    auto it = ID.find({req.source.ip, req.source.port});
    if(it == ID.end()) cout << "\nSender is not in your List for some reason"<<endl;
    else{
        string reply = "";
        for(File &f:myFiles){
            if(match(f.filename, req.payload)) reply += f.filename + "\n";
        }
        string id = to_string(it->second);
        if(reply.size()) msg(reply, 1, id);
    }
}

void readSearchReply(string &filenames, int id){
    stringstream ss(filenames);
    string filename;
    cout<<endl;
    while(getline(ss, filename)) printf("- %s by %s(%d)\n", filename.c_str(), peer[id].name.c_str(), id);
    fflush(stdout);
}

void readRequest(string &filename, int id){
    printf("\n-> %s(%d) requested for file - %s\n", peer[id].name.c_str(), id, filename.c_str());
    fflush(stdout);
}

void readSentFile(string &payload, int id){
    stringstream ss(payload);
    string filename, contents;
    if(!getline(ss, filename, '#')) cout<<"\nReceived Filename not found"<<endl;
    else if(!getline(ss, contents, '\0')) cout<<"\nReceived File is empty"<<endl;
    else{
        printf("\n-> %s(%d) sent file - %s\n", peer[id].name.c_str(), id, filename.c_str());
        fflush(stdout);
        myFiles.push_back(File(filename, contents));
    }
}

bool ClientRead(int client_fd){
    string msg = Read(client_fd);
    Message request(msg);
    if(seen_messages.find(request.id) != seen_messages.end()){      // Already seen this message. Similar to visited array in DFS.
        // debug("Seen - "+to_string(request.id));
        return true;  
    }
    seen_messages.insert(request.id);

    // debug("ClientRead: " + msg);
    
    bool keep = true;                                               // returns false if client is Disconnected  
    if(request.destination == my){                                  // Message reached its fixed destination.
        auto it = ID.find({request.source.ip, request.source.port});
        if(it == ID.end()){
            cout << "\nSender is not in your List for some reason"<<endl;
            return true;
        }
        int id = it->second;
        if(request.isSearchReply()) readSearchReply(request.payload, id);
        else if(request.isRequest()) readRequest(request.payload, id);
        else if(request.isSendFile()) readSentFile(request.payload, id);
        else peer[ID[{request.source.ip, request.source.port}]].print(request.payload, true);       // General private message
    }
    else{
        if(request.isBroadcast()) peer[ID[{request.source.ip, request.source.port}]].print(request.payload);    // General Broadcasted message
        else if(request.isNewConnection()) readNewConnection(request);                              // New Client connected
        else if(request.isNewDisconnection()) keep = readNewDisconnection(request, client_fd);      // Client disconnected
        else if(request.isSearch()) readSearch(request);
        
        request.ttl--;                                                                              // Reduce hop limit by 1 after each Read();
        if(request.ttl > 0) forward(request, client_fd);                                            // Keep Forwarding.
    }
    return keep;
}

// File Sharing START

void allfiles(string &regex){
    getallFiles();
    int count = 0;
    cout<<"\nAvailable Files - "<<endl;
    for(File &f:allFiles){    
        if(match(f.filename, regex)) cout << (++count) << ". " << f.filename << endl;
    }
    if(!count) cout<<"No files here"<<endl;
}

void removefile(string &filename, bool close=false){
    if((!close) && (Hash(filename)==my.hash())){
        cout<<"You can't remove this file"<<endl;
        return;
    }
    for (auto it = myFiles.begin(); it != myFiles.end(); ++it) {
        if (it->filename == filename) {
            it->Remove();
            myFiles.erase(it);
            return;
        }
    }
}

void myfiles(string &regex){
    cout<<"\nYour Files:- "<<endl;
    int count = 0;
    for(File &f:myFiles){
        if(match(f.filename, regex)) cout<<(++count)<<". "<<f.filename<<endl;
    }
    if(!count) cout<<"You have no files matching given regex.\nUse /request to get files."<<endl;
}

void readfile(string &filename){
    auto it=find(myFiles.begin(),myFiles.end(),filename);
    if(it==myFiles.end()) cout<<"\nYou don't have access to this file.\nUse /request to get the file."<<endl;
    else cout<<endl<<it->open()<<endl;
}

void search(string &regex){
    msg(regex, -4);
    cout<<"\nSearch Results:-"<<endl;
}

void request(string &filename){         // Hash filename to find required destination
    for(int id=0;id<peer.size();id++){
        if(peer[id].reachable && (Hash(filename)==peer[id].hash())) msg(filename, 2, to_string(id));    // Can send to multiple owners
    }
}

void File::Send(string &id){
    string payload = filename + "#" + open();
    msg(payload, 3, id);
}

void sendfile(string &id, string &filename){
    debug("sendfile - " + id + " , " + filename);
    auto it=find(myFiles.begin(),myFiles.end(),filename);
    if(it==myFiles.end()) cout<<"\nYou don't have access to this file.\nUse /request to get the file."<<endl;
    else it->Send(id);
}

// File Sharing END

void Exit(){
    castNewDisconnection();                                 // Announce to everyone that you disconnected
    for(File &f:myFiles) f.Remove();                        // Remove 1 from .owners of all files
    if(rmdir(my_path.c_str())) error("Deleting myfiles");   // Deleting myfiles after program ends
}

bool Command(){
    string input;
    getline(cin, input);

    // Divide input comamnd into parts
    string command,x,y;
    istringstream iss(input);
    iss>>command>>x;
    getline(iss>>ws, y);            // Consumes extra whitespace

    // Chat Commands
    if(command=="/help") help();
    else if(command=="/list") List(x=="neighbors");
    else if(command=="/connect") Connect(x,y);
    else if(command=="/msg") msg(y, 0, x);
    else if(command=="/broadcast"){
        x+=y;
        msg(x, -1);
    }
    else if(command=="/exit") {
        Exit();
        return false;                               // Exit program
    }
    // File Sharing
    else if(command == "/allfiles") allfiles(x+=y);
    else if(command == "/removefile") removefile(x+=y);
    else if(command == "/myfiles") myfiles(x+=y);
    else if(command == "/allfiles") allfiles(x+=y);
    else if(command == "/readfile") readfile(x+=y);
    else if(command == "/search") search(x+=y);
    else if(command == "/request") request(x+=y);
    else if(command == "/sendfile") sendfile(x,y);
    else msg(input, -1);                           // Broadcast by default

    return true;
}

void setup_fd(){                    // Basic fd_set setup
    FD_ZERO(&all_neighbors);
    FD_SET(STDIN, &all_neighbors);
    FD_SET(my_fd, &all_neighbors);
    max_fd = my_fd;
}

void start_server(int port){
    my_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(my_fd<0) error("Socket - " + to_string(my_fd),true);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if(bind(my_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0) error("Bind - " + to_string(port),true);

    string name;
    cout<<"\nEnter your name: ";
    getline(cin, name);

    // Get my current ip
    char host[256];
    int hostname = gethostname(host, sizeof(host));
    struct hostent *host_entry = gethostbyname(host);
    char *IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

    int id = ID[{string(IP),port}] = ID.size();
    peer.push_back(Peer(id, name, string(IP), port));

    listen(my_fd, 5);
    printf("\nP2P Chat started on [ Port:%d   IP:%s ]\n",port,IP);
    fflush(stdout);
}

int main(int argc, char *argv[]){
    srand(time(NULL));

    if((argc>2) && (argv[2][0]=='1')) DEBUG = true;    // debug() enabled only if a third compile time argument = 1 is given.
    else if(argc < 2) error("No port provided",true);
    int port = atoi(argv[1]);

    start_server(port);
    setup_fd();

    getallFiles();
    if(mkdir(my_path.c_str(), 0755)) error("Creating myfiles", true);      // Create myfiles-<>-<> directory
    updatemyFiles(1);

    help();
    cout<<"\nYou: ";
    fflush(stdout);

    while(true){
        int prev_n=peers;
        fd_set readset = all_neighbors;
        // Returns the set of currently ready to read clients / user-input / connections
        int ready = select(max_fd+1, &readset, NULL, NULL, NULL);
        if(ready < 0) error("Select - " + to_string(ready)); 
        
        if(FD_ISSET(STDIN, &readset)){
            // debug("Input Ready");
            if(!Command()) break;
        }
        if(FD_ISSET(my_fd, &readset)){
            // debug("Connection Ready");
            ready--;
            Accept();
        }

        vector<int> disconnected_clients;
        for(int client_fd: neighbor_fds){
            if(!FD_ISSET(client_fd, &readset)) continue;                            // Client has nothing to send.
            // debug("Client Ready");
            if(!ClientRead(client_fd)) disconnected_clients.push_back(client_fd);   // Client got disconnected.
        }

        if(ready){
            // debug("Ready - " + to_string(ready));
            cout<<"\nYou: ";
            cout.flush();
        }

        for(int fd:disconnected_clients) remove_neighbor(fd);
        
        peers=0;
        for(Peer &p:peer) peers += p.reachable;
        debug(to_string(peers) + " peers");
        if(prev_n!=peers) updatemyFiles(prev_n);
    }
    
    close(my_fd);

    return 0;
}