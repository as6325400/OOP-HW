#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <climits>
#include <functional>
#include <iomanip>
#include <stack>
#include <assert.h>
#include <bits/stdc++.h>

using namespace std;

#define SET(func_name,type,var_name,_var_name) void func_name(type _var_name) { var_name = _var_name ;} 
#define GET(func_name,type,var_name) type func_name() const { return var_name ;}

class header;
class payload;
class packet;
class node;
class event;
class link; // new

// for simplicity, we use a const int to simulate the delay
// if you want to simulate the more details, you should revise it to be a class
const unsigned int ONE_HOP_DELAY = 10;
const unsigned int BROCAST_ID = UINT_MAX;

// BROCAST_ID means that all neighbors are receivers; UINT_MAX is the maximum value of unsigned int

class header {
    public:
        virtual ~header() {}

        SET(setSrcID, unsigned int , srcID, _srcID);
        SET(setDstID, unsigned int , dstID, _dstID);
        SET(setPreID, unsigned int , preID, _preID);
        SET(setNexID, unsigned int , nexID, _nexID);
        GET(getSrcID, unsigned int , srcID);
        GET(getDstID, unsigned int , dstID);
        GET(getPreID, unsigned int , preID);
        GET(getNexID, unsigned int , nexID);
        
        virtual string type() = 0;
        
        // factory concept: generate a header
        class header_generator {
                // lock the copy constructor
                header_generator(header_generator &){}
                // store all possible types of header
                static map<string,header_generator*> prototypes;
            protected:
                // allow derived class to use it
                header_generator() {}
                // after you create a new header type, please register the factory of this header type by this function
                void register_header_type(header_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your header
                virtual header* generate() = 0 ;
            public:
                // you have to implement your own type() to return your header type
        	    virtual string type() = 0 ;
        	    // this function is used to generate any type of header derived
        	    static header * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such header type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered header types: " << endl;
            	    for (map<string,header::header_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~header_generator(){};
        };
        
    protected:
        header():srcID(BROCAST_ID),dstID(BROCAST_ID),preID(BROCAST_ID),nexID(BROCAST_ID){} // this constructor cannot be directly called by users

    private:
        unsigned int srcID;
        unsigned int dstID;
        unsigned int preID;
        unsigned int nexID;
        header(header&){} // this constructor cannot be directly called by users
};
map<string,header::header_generator*> header::header_generator::prototypes;

class TRA_data_header : public header{
        TRA_data_header(TRA_data_header&){} // cannot be called by users
        
    protected:
        TRA_data_header(){} // this constructor cannot be directly called by users

    public:
        ~TRA_data_header(){}
        string type() { return "TRA_data_header"; }

        class TRA_data_header_generator;
        friend class TRA_data_header_generator;
        // TRA_data_header_generator is derived from header_generator to generate a header
        class TRA_data_header_generator : public header_generator{
                static TRA_data_header_generator sample;
                // this constructor is only for sample to register this header type
                TRA_data_header_generator() { /*cout << "TRA_data_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "TRA_data_header generated" << endl;
                    return new TRA_data_header; 
                }
            public:
                virtual string type() { return "TRA_data_header";}
                ~TRA_data_header_generator(){}
        
        };
};
TRA_data_header::TRA_data_header_generator TRA_data_header::TRA_data_header_generator::sample;

class TRA_ctrl_header : public header{
        TRA_ctrl_header(TRA_ctrl_header&){} // cannot be called by users
        
    protected:
        TRA_ctrl_header(){} // this constructor cannot be directly called by users

    public:
        ~TRA_ctrl_header(){}
        string type() { return "TRA_ctrl_header"; }

        class TRA_ctrl_header_generator;
        friend class TRA_ctrl_header_generator;
        // TRA_ctrl_header_generator is derived from header_generator to generate a header
        class TRA_ctrl_header_generator : public header_generator{
                static TRA_ctrl_header_generator sample;
                // this constructor is only for sample to register this header type
                TRA_ctrl_header_generator() { /*cout << "TRA_ctrl_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "TRA_ctrl_header generated" << endl;
                    return new TRA_ctrl_header; 
                }
            public:
                virtual string type() { return "TRA_ctrl_header";}
                ~TRA_ctrl_header_generator(){}
        
        };
};
TRA_ctrl_header::TRA_ctrl_header_generator TRA_ctrl_header::TRA_ctrl_header_generator::sample;


class SDN_ctrl_header : public header{
        SDN_ctrl_header(SDN_ctrl_header&){} // cannot be called by users
        
    protected:
        SDN_ctrl_header(){} // this constructor cannot be directly called by users

    public:
        ~SDN_ctrl_header(){}
        string type() { return "SDN_ctrl_header"; }

        class SDN_ctrl_header_generator;
        friend class SDN_ctrl_header_generator;
        // SDN_ctrl_header_generator is derived from header_generator to generate a header
        class SDN_ctrl_header_generator : public header_generator{
                static SDN_ctrl_header_generator sample;
                // this constructor is only for sample to register this header type
                SDN_ctrl_header_generator() { /*cout << "SDN_ctrl_header registered" << endl;*/ register_header_type(&sample); }
            protected:
                virtual header * generate(){
                    // cout << "SDN_ctrl_header generated" << endl;
                    return new SDN_ctrl_header; 
                }
            public:
                virtual string type() { return "SDN_ctrl_header";}
                ~SDN_ctrl_header_generator(){}
        
        };
};
SDN_ctrl_header::SDN_ctrl_header_generator SDN_ctrl_header::SDN_ctrl_header_generator::sample;



class payload {
        payload(payload&){} // this constructor cannot be directly called by users
        
        string msg;
        
    protected:
        payload(){}
    public:
        virtual ~payload(){}
        virtual string type() = 0;
        
        SET(setMsg,string,msg,_msg);
        GET(getMsg,string,msg);
        
        class payload_generator {
                // lock the copy constructor
                payload_generator(payload_generator &){}
                // store all possible types of header
                static map<string,payload_generator*> prototypes;
            protected:
                // allow derived class to use it
                payload_generator() {}
                // after you create a new payload type, please register the factory of this payload type by this function
                void register_payload_type(payload_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your payload
                virtual payload* generate() = 0;
            public:
                // you have to implement your own type() to return your header type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of header derived
        	    static payload * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such payload type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered payload types: " << endl;
            	    for (map<string,payload::payload_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~payload_generator(){};
        };
};
map<string,payload::payload_generator*> payload::payload_generator::prototypes;


class TRA_data_payload : public payload {
        TRA_data_payload(TRA_data_payload&){}

    protected:
        TRA_data_payload(){} // this constructor cannot be directly called by users
    public:
        ~TRA_data_payload(){}
        
        string type() { return "TRA_data_payload"; }
        
        class TRA_data_payload_generator;
        friend class TRA_data_payload_generator;
        // TRA_data_payload is derived from payload_generator to generate a payload
        class TRA_data_payload_generator : public payload_generator{
                static TRA_data_payload_generator sample;
                // this constructor is only for sample to register this payload type
                TRA_data_payload_generator() { /*cout << "TRA_data_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "TRA_data_payload generated" << endl;
                    return new TRA_data_payload; 
                }
            public:
                virtual string type() { return "TRA_data_payload";}
                ~TRA_data_payload_generator(){}
        };
};
TRA_data_payload::TRA_data_payload_generator TRA_data_payload::TRA_data_payload_generator::sample;

class TRA_ctrl_payload : public payload {
        TRA_ctrl_payload(TRA_ctrl_payload & s): counter (s.counter) {}
        
        unsigned int counter ;
        
    protected:
        TRA_ctrl_payload(): counter (0) {} // this constructor cannot be directly called by users
    public:
        ~TRA_ctrl_payload(){}
        
        void increase() { counter ++; } // used to increase the counter
        GET(getCounter,unsigned int,counter); // used to get the value of counter
        
        string type() { return "TRA_ctrl_payload"; }
        
        
        class TRA_ctrl_payload_generator;
        friend class TRA_ctrl_payload_generator;
        // TRA_data_payload is derived from payload_generator to generate a payload
        class TRA_ctrl_payload_generator : public payload_generator{
                static TRA_ctrl_payload_generator sample;
                // this constructor is only for sample to register this payload type
                TRA_ctrl_payload_generator() { /*cout << "TRA_ctrl_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "TRA_ctrl_payload generated" << endl;
                    return new TRA_ctrl_payload; 
                }
            public:
                virtual string type() { return "TRA_ctrl_payload";}
                ~TRA_ctrl_payload_generator(){}
        };
};
TRA_ctrl_payload::TRA_ctrl_payload_generator TRA_ctrl_payload::TRA_ctrl_payload_generator::sample;

class SDN_ctrl_payload : public payload {
        SDN_ctrl_payload(SDN_ctrl_payload&){}
        
        unsigned int matID; // match: target
        unsigned int actID; // action: the next hop
        double per; // percentage

    protected:
        SDN_ctrl_payload(){} // this constructor cannot be directly called by users
    public:
        ~SDN_ctrl_payload(){}

        string type() { return "SDN_ctrl_payload"; }
        
        SET(setMatID,unsigned int,matID,_matID);
        GET(getMatID,unsigned int,matID);
        SET(setActID,unsigned int,actID,_actID);
        GET(getActID,unsigned int,actID);
        SET(setPer,double,per,_per);
        GET(getPer,double,per);
        
        
        class SDN_ctrl_payload_generator;
        friend class SDN_ctrl_payload_generator;
        // SDN_data_payload is derived from payload_generator to generate a payload
        class SDN_ctrl_payload_generator : public payload_generator{
                static SDN_ctrl_payload_generator sample;
                // this constructor is only for sample to register this payload type
                SDN_ctrl_payload_generator() { /*cout << "SDN_ctrl_payload registered" << endl;*/ register_payload_type(&sample); }
            protected:
                virtual payload * generate(){ 
                    // cout << "SDN_ctrl_payload generated" << endl;
                    return new SDN_ctrl_payload; 
                }
            public:
                virtual string type() { return "SDN_ctrl_payload";}
                ~SDN_ctrl_payload_generator(){}
        };
};
SDN_ctrl_payload::SDN_ctrl_payload_generator SDN_ctrl_payload::SDN_ctrl_payload_generator::sample;


class packet{
        // a packet usually contains a header and a payload
        header *hdr;
        payload *pld;
        unsigned int p_id;
        static unsigned int last_packet_id ; 
        
        packet(packet &) {}
        static int live_packet_num ;
    protected:
        // these constructors cannot be directly called by users
        packet(): hdr(nullptr), pld(nullptr) { p_id=last_packet_id++; live_packet_num ++; }
        packet(string _hdr, string _pld, bool rep = false, unsigned int rep_id = 0) {
            if (! rep ) // a duplicated packet does not have a new packet id
                p_id = last_packet_id ++;
            else
                p_id = rep_id;
            hdr = header::header_generator::generate(_hdr); 
            pld = payload::payload_generator::generate(_pld); 
            live_packet_num ++;
        }
    public:
        virtual ~packet(){ 
            // cout << "packet destructor begin" << endl;
            if (hdr != nullptr) 
                delete hdr; 
            if (pld != nullptr) 
                delete pld; 
            live_packet_num --;
            // cout << "packet destructor end" << endl;
        }
        
        SET(setHeader,header*,hdr,_hdr);
        GET(getHeader,header*,hdr);
        SET(setPayload,payload*,pld,_pld);
        GET(getPayload,payload*,pld);
        GET(getPacketID,unsigned int,p_id);
        
        static void discard ( packet* &p ) {
            // cout << "checking" << endl;
            if (p != nullptr) {
                // cout << "discarding" << endl;
                // cout << p->type() << endl;
                delete p;
                // cout << "discarded" << endl;
            }
            p = nullptr;
            // cout << "checked" << endl;
        }
        virtual string type () = 0;
        virtual string addition_information() { return ""; }
        
        static int getLivePacketNum () { return live_packet_num; }
        
        class packet_generator;
        friend class packet_generator;
        class packet_generator {
                // lock the copy constructor
                packet_generator(packet_generator &){}
                // store all possible types of packet
                static map<string,packet_generator*> prototypes;
            protected:
                // allow derived class to use it
                packet_generator() {}
                // after you create a new packet type, please register the factory of this payload type by this function
                void register_packet_type(packet_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your payload
                virtual packet* generate ( packet *p = nullptr) = 0;
            public:
                // you have to implement your own type() to return your packet type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of packet derived
        	    static packet * generate (string type) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            			return prototypes[type]->generate(); // generate it!!
            		}
            		std::cerr << "no such packet type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static packet * replicate (packet *p) {
            	    if(prototypes.find(p->type()) != prototypes.end()){ // if this type derived exists 
            			return prototypes[p->type()]->generate(p); // generate it!!
            		}
            		std::cerr << "no such packet type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered packet types: " << endl;
            	    for (map<string,packet::packet_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~packet_generator(){};
        };
};
map<string,packet::packet_generator*> packet::packet_generator::prototypes;
unsigned int packet::last_packet_id = 0 ;
int packet::live_packet_num = 0;


// this packet is used to tell the destination the msg
class TRA_data_packet: public packet {
        TRA_data_packet(TRA_data_packet &) {}
        
    protected:
        TRA_data_packet(){} // this constructor cannot be directly called by users
        TRA_data_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<TRA_data_header*>(this->getHeader())) = *(dynamic_cast<TRA_data_header*> (p->getHeader()));
            *(dynamic_cast<TRA_data_payload*>(this->getPayload())) = *(dynamic_cast<TRA_data_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<TRA_data_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<TRA_data_header*>(p))->isVisited;
        } // for duplicate
        TRA_data_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~TRA_data_packet(){}
        string type() { return "TRA_data_packet"; }
        
        class TRA_data_packet_generator;
        friend class TRA_data_packet_generator;
        // TRA_data_packet is derived from packet_generator to generate a pub packet
        class TRA_data_packet_generator : public packet_generator{
                static TRA_data_packet_generator sample;
                // this constructor is only for sample to register this packet type
                TRA_data_packet_generator() { /*cout << "TRA_data_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "TRA_data_packet generated" << endl;
                    if ( nullptr == p )
                        return new TRA_data_packet("TRA_data_header","TRA_data_payload"); 
                    else
                        return new TRA_data_packet(p); // duplicate
                }
            public:
                virtual string type() { return "TRA_data_packet";}
                ~TRA_data_packet_generator(){}
        };
};
TRA_data_packet::TRA_data_packet_generator TRA_data_packet::TRA_data_packet_generator::sample;

// this packet type is used to tell the node what should be installed in the routing table
class TRA_ctrl_packet: public packet {
        TRA_ctrl_packet(TRA_ctrl_packet &) {}
        
    protected:
        TRA_ctrl_packet(){} // this constructor cannot be directly called by users
        TRA_ctrl_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<TRA_ctrl_header*>(this->getHeader())) = *(dynamic_cast<TRA_ctrl_header*> (p->getHeader()));
            *(dynamic_cast<TRA_ctrl_payload*>(this->getPayload())) = *(dynamic_cast<TRA_ctrl_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<TRA_ctrl_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<TRA_ctrl_header*>(p))->isVisited;
        } // for duplicate
        TRA_ctrl_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~TRA_ctrl_packet(){}
        string type() { return "TRA_ctrl_packet"; }
        virtual string addition_information() {
            unsigned int counter = (dynamic_cast<TRA_ctrl_payload*>(this->getPayload()))->getCounter();
            // cout << counter << endl;
            return " counter " + to_string(counter);
        }
        
        class TRA_ctrl_packet_generator;
        friend class TRA_ctrl_packet_generator;
        // TRA_ctrl_packet is derived from packet_generator to generate a pub packet
        class TRA_ctrl_packet_generator : public packet_generator{
                static TRA_ctrl_packet_generator sample;
                // this constructor is only for sample to register this packet type
                TRA_ctrl_packet_generator() { /*cout << "TRA_ctrl_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "TRA_ctrl_packet generated" << endl;
                    if ( nullptr == p )
                        return new TRA_ctrl_packet("TRA_ctrl_header","TRA_ctrl_payload"); 
                    else
                        return new TRA_ctrl_packet(p); // duplicate
                }
            public:
                virtual string type() { return "TRA_ctrl_packet";}
                ~TRA_ctrl_packet_generator(){}
        };
};
TRA_ctrl_packet::TRA_ctrl_packet_generator TRA_ctrl_packet::TRA_ctrl_packet_generator::sample;


class SDN_ctrl_packet: public packet {
        SDN_ctrl_packet(SDN_ctrl_packet &) {}
        
    protected:
        SDN_ctrl_packet(){} // this constructor cannot be directly called by users
        SDN_ctrl_packet(packet*p): packet(p->getHeader()->type(), p->getPayload()->type(), true, p->getPacketID()) {
            *(dynamic_cast<SDN_ctrl_header*>(this->getHeader())) = *(dynamic_cast<SDN_ctrl_header*> (p->getHeader()));
            *(dynamic_cast<SDN_ctrl_payload*>(this->getPayload())) = *(dynamic_cast<SDN_ctrl_payload*> (p->getPayload()));
            //DFS_path = (dynamic_cast<SDN_ctrl_header*>(p))->DFS_path;
            //isVisited = (dynamic_cast<SDN_ctrl_header*>(p))->isVisited;
        } // for duplicate
        SDN_ctrl_packet(string _h, string _p): packet(_h,_p) {}
        
    public:
        virtual ~SDN_ctrl_packet(){}
        string type() { return "SDN_ctrl_packet"; }
        
        class SDN_ctrl_packet_generator;
        friend class SDN_ctrl_packet_generator;
        // SDN_ctrl_packet is derived from packet_generator to generate a pub packet
        class SDN_ctrl_packet_generator : public packet_generator{
                static SDN_ctrl_packet_generator sample;
                // this constructor is only for sample to register this packet type
                SDN_ctrl_packet_generator() { /*cout << "SDN_ctrl_packet registered" << endl;*/ register_packet_type(&sample); }
            protected:
                virtual packet *generate (packet *p = nullptr){
                    // cout << "SDN_ctrl_packet generated" << endl;
                    if ( nullptr == p )
                        return new SDN_ctrl_packet("SDN_ctrl_header","SDN_ctrl_payload"); 
                    else
                        return new SDN_ctrl_packet(p); // duplicate
                }
            public:
                virtual string type() { return "SDN_ctrl_packet";}
                ~SDN_ctrl_packet_generator(){}
        };
};
SDN_ctrl_packet::SDN_ctrl_packet_generator SDN_ctrl_packet::SDN_ctrl_packet_generator::sample;




class node {
        // all nodes created in the program
        static map<unsigned int, node*> id_node_table;
        
        unsigned int id;
        map<unsigned int,bool> phy_neighbors;
        
    protected:
        node(node&){} // this constructor should not be used
        node(){} // this constructor should not be used
        node(unsigned int _id): id(_id) { id_node_table[_id] = this; }
    public:
        virtual ~node() { // erase the node
            id_node_table.erase (id) ;
        }
        virtual string type() = 0; // please define it in your derived node class
        
        static void print_all_routing_table(){
            for (auto it = id_node_table.begin(); it != id_node_table.end(); it++){
                // cerr << "node " << it -> first << " routing table: " << node::getNodeNum() << endl;
                if(it -> first == node::getNodeNum() - 1) continue;
                cout << it->first << "\n";
                it -> second -> print_routing_table();
            }
            cout << endl;
        } 

        virtual void print_routing_table(){
            
        }

        void add_phy_neighbor (unsigned int _id, string link_type = "simple_link"); // we only add a directed link from id to _id
        void del_phy_neighbor (unsigned int _id); // we only delete a directed link from id to _id
        
        // you can use the function to get the node's neigbhors at this time
        // but in the project 3, you are not allowed to use this function 
        const map<unsigned int,bool> & getPhyNeighbors () { 
            return phy_neighbors;
        }
        
        
        void recv (packet *p) {
            packet *tp = p;
            recv_handler(tp); 
            packet::discard(p); 
        } // the packet will be directly deleted after the handler
        void send (packet *p);
        
        // receive the packet and do something; this is a pure virtual function
        virtual void recv_handler(packet *p) = 0;
        void send_handler(packet *P);
        
        static node * id_to_node (unsigned int _id) { return ((id_node_table.find(_id)!=id_node_table.end()) ? id_node_table[_id]: nullptr) ; }
        GET(getNodeID,unsigned int,id);
        
        static void del_node (unsigned int _id) {
            if (id_node_table.find(_id) != id_node_table.end())
                id_node_table.erase(_id);
        }
        static unsigned int getNodeNum () { return id_node_table.size(); }

        class node_generator {
                // lock the copy constructor
                node_generator(node_generator &){}
                // store all possible types of node
                static map<string,node_generator*> prototypes;
            protected:
                // allow derived class to use it
                node_generator() {}
                // after you create a new node type, please register the factory of this node type by this function
                void register_node_type(node_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your node
                virtual node* generate(unsigned int _id) = 0;
            public:
                // you have to implement your own type() to return your node type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of node derived
        	    static node * generate (string type, unsigned int _id) {
        	        if(id_node_table.find(_id)!=id_node_table.end()){
        	            std::cerr << "duplicate node id" << std::endl; // node id is duplicated
        	            return nullptr;
        	        }
        	        else if ( BROCAST_ID == _id ) {
        	            cerr << "BROCAST_ID cannot be used" << endl;
        	            return nullptr;
        	        }
            		else if(prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            		    node * created_node = prototypes[type]->generate(_id);
            			return created_node; // generate it!!
            		}
            		std::cerr << "no such node type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered node types: " << endl;
            	    for (map<string,node::node_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~node_generator(){};
        };
};
map<string,node::node_generator*> node::node_generator::prototypes;
map<unsigned int,node*> node::id_node_table;

class TRA_switch: public node {
        // map<unsigned int,bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors 
        
        map<unsigned int, unsigned int> routingTable;
        map<unsigned int, unsigned int> recieveTime;

        // bool hi; // this is used for example; you can remove it when doing hw2

    protected:
        TRA_switch() {} // it should not be used
        TRA_switch(TRA_switch&) {} // it should not be used
        TRA_switch(unsigned int _id): node(_id){} // this constructor cannot be directly called by users
    
    public:
        ~TRA_switch(){}
        string type() { return "TRA_switch"; }
        
        void addNextNode(unsigned int dst, unsigned int nextNode){
            // cout << "dst   " << dst <<' ' << "nextnode   " << nextNode << endl;
            routingTable[dst] = nextNode;
            return;
        }

        // please define recv_handler function to deal with the incoming packet
        virtual void recv_handler (packet *p);
        
        virtual void print_routing_table(){
            for(auto &i : routingTable){
                cout << i.first << " " << i.second << endl;
            }
        }

        // void add_one_hop_neighbor (unsigned int n_id) { one_hop_neighbors[n_id] = true; }
        // unsigned int get_one_hop_neighbor_num () { return one_hop_neighbors.size(); }
        
        class TRA_switch_generator;
        friend class TRA_switch_generator;
        // TRA_switch is derived from node_generator to generate a node
        class TRA_switch_generator : public node_generator{
                static TRA_switch_generator sample;
                // this constructor is only for sample to register this node type
                TRA_switch_generator() { /*cout << "TRA_switch registered" << endl;*/ register_node_type(&sample); }
            protected:
                virtual node * generate(unsigned int _id){ /*cout << "TRA_switch generated" << endl;*/ return new TRA_switch(_id); }
            public:
                virtual string type() { return "TRA_switch";}
                ~TRA_switch_generator(){}
        };
};
TRA_switch::TRA_switch_generator TRA_switch::TRA_switch_generator::sample;


///////////////////////////////////////////////////////////////////////////////////////////////////



class SDN_switch: public node {
        // map<unsigned int,bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors 
        
        // bool hi; // this is used for example; you can remove it when doing hw2

    protected:
        SDN_switch() {} // it should not be used
        SDN_switch(SDN_switch&) {} // it should not be used
        SDN_switch(unsigned int _id): node(_id) {} // this constructor cannot be directly called by users
    
    public:
        map<unsigned int, unsigned int> routingTable;
        map<unsigned int, unsigned int> recieveTime;
        map<unsigned int, vector<pair<int, double>>> table;
        void addNextNode(unsigned int dst, unsigned int nextNode){
            // cout << "dst   " << dst <<' ' << "nextnode   " << nextNode << endl;
            routingTable[dst] = nextNode;
            return;
        }
        ~SDN_switch(){}
        string type() { return "SDN_switch"; }
        
        // please define recv_handler function to deal with the incoming packet
        virtual void recv_handler (packet *p);
        void print_routing_table(){
            for(auto &i : table){
                cout << i.first << " ";
                for(auto &j : i.second){
                    if(j.second == 0) continue;
                    cout << j.first << " " << j.second * 100<< "% ";
                }
                cout << '\n';
            }
        }
        // void add_one_hop_neighbor (unsigned int n_id) { one_hop_neighbors[n_id] = true; }
        // unsigned int get_one_hop_neighbor_num () { return one_hop_neighbors.size(); }
        
        class SDN_switch_generator;
        friend class SDN_switch_generator;
        // SDN_switch is derived from node_generator to generate a node
        class SDN_switch_generator : public node_generator{
                static SDN_switch_generator sample;
                // this constructor is only for sample to register this node type
                SDN_switch_generator() { /*cout << "SDN_switch registered" << endl;*/ register_node_type(&sample); }
            protected:
                virtual node * generate(unsigned int _id){ /*cout << "SDN_switch generated" << endl;*/ return new SDN_switch(_id); }
            public:
                virtual string type() { return "SDN_switch";}
                ~SDN_switch_generator(){}
        };
};
SDN_switch::SDN_switch_generator SDN_switch::SDN_switch_generator::sample;


/////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////



class SDN_controller: public node {
        // map<unsigned int,bool> one_hop_neighbors; // you can use this variable to record the node's 1-hop neighbors 
        
        // bool hi; // this is used for example; you can remove it when doing hw2

    protected:
        SDN_controller() {} // it should not be used
        SDN_controller(SDN_controller&) {} // it should not be used
        SDN_controller(unsigned int _id): node(_id) {} // this constructor cannot be directly called by users
    
    public:
        ~SDN_controller(){}
        string type() { return "SDN_controller"; }
        
        // please define recv_handler function to deal with the incoming packet
        virtual void recv_handler (packet *p);
        
        // void add_one_hop_neighbor (unsigned int n_id) { one_hop_neighbors[n_id] = true; }
        // unsigned int get_one_hop_neighbor_num () { return one_hop_neighbors.size(); }
        
        class SDN_controller_generator;
        friend class SDN_controller_generator;
        // SDN_controller is derived from node_generator to generate a node
        class SDN_controller_generator : public node_generator{
                static SDN_controller_generator sample;
                // this constructor is only for sample to register this node type
                SDN_controller_generator() { /*cout << "SDN_controller registered" << endl;*/ register_node_type(&sample); }
            protected:
                virtual node * generate(unsigned int _id){ /*cout << "SDN_controller generated" << endl;*/ return new SDN_controller(_id); }
            public:
                virtual string type() { return "SDN_controller";}
                ~SDN_controller_generator(){}
        };
};
SDN_controller::SDN_controller_generator SDN_controller::SDN_controller_generator::sample;


/////////////////////////////////////////////////////////////////////////////////////////////////

class mycomp {
    bool reverse;
    
    public:
        mycomp(const bool& revparam = false) { reverse=revparam ; }
        bool operator() (const event* lhs, const event* rhs) const;
};

class event {
        event(event*&){} // this constructor cannot be directly called by users
        static priority_queue < event*, vector < event* >, mycomp > events;
        static unsigned int cur_time; // timer
        static unsigned int end_time;
        
        // get the next event
        static event * get_next_event() ;
        static void add_event (event *e) { events.push(e); }
        static hash<string> event_seq;
        
    protected:
        unsigned int trigger_time;
        
        event(){} // it should not be used
        event(unsigned int _trigger_time): trigger_time(_trigger_time) {}

    public:
        virtual void trigger()=0;
        virtual ~event(){}

        virtual unsigned int event_priority() const = 0;
        unsigned int get_hash_value(string string_for_hash) const {
            unsigned int priority = event_seq (string_for_hash);
            return priority;
        }
        
        static void flush_events (); // only for debug
        
        GET(getTriggerTime,unsigned int,trigger_time);
        
        static void start_simulate( unsigned int _end_time ); // the function is used to start the simulation
        
        static unsigned int getCurTime() { return cur_time ; }
        static void getCurTime(unsigned int _cur_time) { cur_time = _cur_time; } 
        // static unsigned int getEndTime() { return end_time ; }
        // static void getEndTime(unsigned int _end_time) { end_time = _end_time; }
        
        virtual void print () const = 0; // the function is used to print the event information

        class event_generator{
                // lock the copy constructor
                event_generator(event_generator &){}
                // store all possible types of event
                static map<string,event_generator*> prototypes;
            protected:
                // allow derived class to use it
                event_generator() {}
                // after you create a new event type, please register the factory of this event type by this function
                void register_event_type(event_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your event
                virtual event* generate(unsigned int _trigger_time, void * data) = 0;
            public:
                // you have to implement your own type() to return your event type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of event derived
        	    static event * generate (string type, unsigned int _trigger_time, void * data) {
            		if(prototypes.find(type) != prototypes.end()){ // if this type derived exists
            		    event * e = prototypes[type]->generate(_trigger_time, data);
            		    add_event(e);
            		    return e; // generate it!!
            		}
            		std::cerr << "no such event type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered event types: " << endl;
            	    for (map<string,event::event_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~event_generator(){}
        };
};
map<string,event::event_generator*> event::event_generator::prototypes;
priority_queue < event*, vector< event* >, mycomp > event::events;
hash<string> event::event_seq;

unsigned int event::cur_time = 0;
unsigned int event::end_time = 0;

void event::flush_events()
{ 
    cout << "**flush begin" << endl;
    while ( ! events.empty() ) {
        cout << setw(11) << events.top()->trigger_time << ": " << setw(11) << events.top()->event_priority() << endl;
        delete events.top();
        events.pop();
    }
    cout << "**flush end" << endl;
}
event * event::get_next_event() {
    if(events.empty()) 
        return nullptr; 
    event * e = events.top();
    events.pop(); 
    // cout << events.size() << " events remains" << endl;
    return e; 
}
void event::start_simulate(unsigned int _end_time) {
    if (_end_time<0) {
        cerr << "you should give a possitive value of _end_time" << endl;
        return;
    }
    end_time = _end_time;
    event *e; 
    e = event::get_next_event ();
    while ( e != nullptr && e->trigger_time <= end_time ) {
        if ( cur_time <= e->trigger_time )
            cur_time = e->trigger_time;
        else {
            cerr << "cur_time = " << cur_time << ", event trigger_time = " << e->trigger_time << endl;
            break;
        }

        // cout << "event trigger_time = " << e->trigger_time << endl;
        e->print(); // for log
        // cout << " event begin" << endl;
        e->trigger();
        // cout << " event end" << endl;
        delete e;
        e = event::get_next_event ();
    }
    // cout << "no more event" << endl;
}

bool mycomp::operator() (const event* lhs, const event* rhs) const {
    // cout << lhs->getTriggerTime() << ", " << rhs->getTriggerTime() << endl;
    // cout << lhs->type() << ", " << rhs->type() << endl;
    unsigned int lhs_pri = lhs->event_priority();
    unsigned int rhs_pri = rhs->event_priority();
    // cout << "lhs hash = " << lhs_pri << endl;
    // cout << "rhs hash = " << rhs_pri << endl;
    
    if (reverse) 
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri < rhs_pri): ((lhs->getTriggerTime()) < (rhs->getTriggerTime()));
    else 
        return ((lhs->getTriggerTime()) == (rhs->getTriggerTime())) ? (lhs_pri > rhs_pri): ((lhs->getTriggerTime()) > (rhs->getTriggerTime()));
}

class recv_event: public event {
    public:
        class recv_data; // forward declaration
            
    private:
        recv_event(recv_event&) {} // this constructor cannot be used
        recv_event() {} // we don't allow users to new a recv_event by themselv
        unsigned int senderID; // the sender
        unsigned int receiverID; // the receiver; the packet will be given to the receiver
        packet *pkt; // the packet
        
    protected:
        // this constructor cannot be directly called by users; only by generator
        recv_event(unsigned int _trigger_time, void *data): event(_trigger_time), senderID(BROCAST_ID), receiverID(BROCAST_ID), pkt(nullptr){
            recv_data * data_ptr = (recv_data*) data;
            senderID = data_ptr->s_id;
            receiverID = data_ptr->r_id; // the packet will be given to the receiver
            pkt = data_ptr->_pkt;
        } 
        
    public:
        virtual ~recv_event(){}
        // recv_event will trigger the recv function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class recv_event_generator;
        friend class recv_event_generator;
        // recv_event is derived from event_generator to generate a event
        class recv_event_generator : public event_generator{
                static recv_event_generator sample;
                // this constructor is only for sample to register this event type
                recv_event_generator() { /*cout << "recv_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "recv_event generated" << endl; 
                    return new recv_event(_trigger_time, data); 
                }
                
            public:
                virtual string type() { return "recv_event";}
                ~recv_event_generator(){}
        };
        // this class is used to initialize the recv_event
        class recv_data{
            public:
                unsigned int s_id;
                unsigned int r_id;
                packet *_pkt;
        };
        
        void print () const;
};
recv_event::recv_event_generator recv_event::recv_event_generator::sample;

void recv_event::trigger() {
    if (pkt == nullptr) {
        cerr << "recv_event error: no pkt!" << endl; 
        return ; 
    }
    else if (node::id_to_node(receiverID) == nullptr){
        cerr << "recv_event error: no node " << receiverID << "!" << endl;
        delete pkt; return ;
    }
    node::id_to_node(receiverID)->recv(pkt); 
}
unsigned int recv_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string (receiverID) + to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the recv_event::print() function is used for log file
void recv_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "   recID"       << setw(11) << receiverID 
         << "   pktID"       << setw(11) << pkt->getPacketID()
         << "   srcID"       << setw(11) << pkt->getHeader()->getSrcID() 
         << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
         << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
         << "   "            << pkt->type()
         << pkt->addition_information();
//  if ( pkt->type() == "TRA_ctrl_packet" ) cout << "   " << ((TRA_ctrl_payload*)pkt->getPayload())->getCounter();
         cout << endl;
    // cout << pkt->type()
    //      << "   time "       << setw(11) << event::getCurTime() 
    //      << "   recID "      << setw(11) << receiverID 
    //      << "   pktID"       << setw(11) << pkt->getPacketID()
    //      << "   srcID "      << setw(11) << pkt->getHeader()->getSrcID() 
    //      << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
    //      << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
    //      << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
    //      << endl;
}

class send_event: public event {
    public:
        class send_data; // forward declaration
            
    private:
        send_event (send_event &){}
        send_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int senderID; // the sender
        unsigned int receiverID; // the receiver 
        packet *pkt; // the packet
    
    protected:
        send_event (unsigned int _trigger_time, void *data): event(_trigger_time), senderID(BROCAST_ID), receiverID(BROCAST_ID), pkt(nullptr){
            send_data * data_ptr = (send_data*) data;
            senderID = data_ptr->s_id;
            receiverID = data_ptr->r_id;
            pkt = data_ptr->_pkt;
        } 
        
    public:
        virtual ~send_event(){}
        // send_event will trigger the send function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class send_event_generator;
        friend class send_event_generator;
        // send_event is derived from event_generator to generate a event
        class send_event_generator : public event_generator{
                static send_event_generator sample;
                // this constructor is only for sample to register this event type
                send_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new send_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "send_event";}
                ~send_event_generator(){}
        };
        // this class is used to initialize the send_event
        class send_data{
            public:
                unsigned int s_id;
                unsigned int r_id;
                packet *_pkt;
                unsigned int t;
        };
        
        void print () const;
};
send_event::send_event_generator send_event::send_event_generator::sample;

void send_event::trigger() {
    if (pkt == nullptr) {
        cerr << "send_event error: no pkt!" << endl; 
        return ; 
    }
    else if (node::id_to_node(senderID) == nullptr){
        cerr << "send_event error: no node " << senderID << "!" << endl;
        delete pkt; return ;
    }
    node::id_to_node(senderID)->send(pkt);
}
unsigned int send_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(senderID) + to_string (receiverID) + to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the send_event::print() function is used for log file
void send_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "   senID"       << setw(11) << senderID 
         << "   pktID"       << setw(11) << pkt->getPacketID()
         << "   srcID"       << setw(11) << pkt->getHeader()->getSrcID() 
         << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
         << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
         << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
         << "   "            << pkt->type()
         << pkt->addition_information()
         //<< "   msg"         << setw(11) << dynamic_cast<TRA_data_payload*>(pkt->getPayload())->getMsg()
         << endl;
    // cout << pkt->type()
    //      << "   time "       << setw(11) << event::getCurTime() 
    //      << "   senID "      << setw(11) << senderID
    //      << "   pktID"       << setw(11) << pkt->getPacketID()
    //      << "   srcID "      << setw(11) << pkt->getHeader()->getSrcID() 
    //      << "   dstID"       << setw(11) << pkt->getHeader()->getDstID() 
    //      << "   preID"       << setw(11) << pkt->getHeader()->getPreID()
    //      << "   nexID"       << setw(11) << pkt->getHeader()->getNexID()
    //      << endl;
}

////////////////////////////////////////////////////////////////////////////////

class TRA_data_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        TRA_data_pkt_gen_event (TRA_data_pkt_gen_event &){}
        TRA_data_pkt_gen_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        // packet *pkt; // the packet
        string msg;
    
    protected:
        TRA_data_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROCAST_ID), dst(BROCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            // pkt = data_ptr->_pkt;
            msg = data_ptr->msg;
        } 
        
    public:
        virtual ~TRA_data_pkt_gen_event(){}
        // TRA_data_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class TRA_data_pkt_gen_event_generator;
        friend class TRA_data_pkt_gen_event_generator;
        // TRA_data_pkt_gen_event_generator is derived from event_generator to generate an event
        class TRA_data_pkt_gen_event_generator : public event_generator{
                static TRA_data_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                TRA_data_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new TRA_data_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "TRA_data_pkt_gen_event";}
                ~TRA_data_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the TRA_data_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id;
                unsigned int dst_id;
                string msg;
                // packet *_pkt;
        };
        
        void print () const;
};
TRA_data_pkt_gen_event::TRA_data_pkt_gen_event_generator TRA_data_pkt_gen_event::TRA_data_pkt_gen_event_generator::sample;

void TRA_data_pkt_gen_event::trigger() {
    if (node::id_to_node(src) == nullptr){
        cerr << "TRA_data_pkt_gen_event error: no node " << src << "!" << endl;
        return ;
    }
    else if ( dst != BROCAST_ID && node::id_to_node(dst) == nullptr ) {
        cerr << "TRA_data_pkt_gen_event error: no node " << dst << "!" << endl;
        return;
    }
    
    TRA_data_packet *pkt = dynamic_cast<TRA_data_packet*> ( packet::packet_generator::generate("TRA_data_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    TRA_data_header *hdr = dynamic_cast<TRA_data_header*> ( pkt->getHeader() );
    TRA_data_payload *pld = dynamic_cast<TRA_data_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src);
    hdr->setDstID(dst);
    hdr->setPreID(src); // this column is not important when the packet is first received by the src (i.e., just generated)
    hdr->setNexID(src); // this column is not important when the packet is first received by the src (i.e., just generated)

    pld->setMsg(msg);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    e_data.r_id = src; // to make the packet start from the src
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event", trigger_time, (void *)&e_data) );

}
unsigned int TRA_data_pkt_gen_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string (dst) ; //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the TRA_data_pkt_gen_event::print() function is used for log file
void TRA_data_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   srcID"       << setw(11) << src
         << "   dstID"       << setw(11) << dst
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   TRA_data_packet generating"
         << endl;
}

class TRA_ctrl_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        TRA_ctrl_pkt_gen_event (TRA_ctrl_pkt_gen_event &){}
        TRA_ctrl_pkt_gen_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        // unsigned int mat;
        // unsigned int act;
        // packet *pkt; // the packet
        string msg;
        // double per; // percentage
    
    protected:
        TRA_ctrl_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROCAST_ID), dst(BROCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            // pkt = data_ptr->_pkt;
            // mat = data_ptr->mat_id;
            // act = data_ptr->act_id;
            msg = data_ptr->msg;
            // per = data_ptr->per;
        } 
        
    public:
        virtual ~TRA_ctrl_pkt_gen_event(){}
        // TRA_ctrl_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class TRA_ctrl_pkt_gen_event_generator;
        friend class TRA_ctrl_pkt_gen_event_generator;
        // TRA_ctrl_pkt_gen_event_generator is derived from event_generator to generate an event
        class TRA_ctrl_pkt_gen_event_generator : public event_generator{
                static TRA_ctrl_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                TRA_ctrl_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new TRA_ctrl_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "TRA_ctrl_pkt_gen_event";}
                ~TRA_ctrl_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the TRA_ctrl_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id; // the controller
                unsigned int dst_id; // the node that should update its rule
                // unsigned int mat_id; // the target of the rule
                // unsigned int act_id; // the next hop toward the target recorded in the rule
                string msg;
                // double per; // the percentage
                // packet *_pkt;
        };
        
        void print () const;
};
TRA_ctrl_pkt_gen_event::TRA_ctrl_pkt_gen_event_generator TRA_ctrl_pkt_gen_event::TRA_ctrl_pkt_gen_event_generator::sample;

void TRA_ctrl_pkt_gen_event::trigger() {
    
    TRA_ctrl_packet *pkt = dynamic_cast<TRA_ctrl_packet*> ( packet::packet_generator::generate("TRA_ctrl_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    TRA_ctrl_header *hdr = dynamic_cast<TRA_ctrl_header*> ( pkt->getHeader() );
    TRA_ctrl_payload *pld = dynamic_cast<TRA_ctrl_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src); 
    hdr->setDstID(dst);
    hdr->setPreID(src);
    // hdr->setNexID(dst); // in hw3, you should set NexID to src
    hdr->setNexID(src);
    
    // payload
    pld->setMsg(msg);
    // pld->setMatID(mat);
    // pld->setActID(act);
    // pld->setPer(per);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    // e_data.r_id = dst; // in hw3, you should set r_id it src
    e_data.r_id = src;
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",trigger_time, (void *)&e_data) );
}
unsigned int TRA_ctrl_pkt_gen_event::event_priority() const {
    string string_for_hash;
    // string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) + to_string(mat) + to_string(act); //to_string (pkt->getPacketID());
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) ; //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the TRA_ctrl_pkt_gen_event::print() function is used for log file
void TRA_ctrl_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
         << "   srcID"       << setw(11) << src
         << "   dstID"       << setw(11) << dst
         << "        "       << setw(11) << " "
         << "        "       << setw(11) << " "
        //  << "   matID"       << setw(11) << mat
        //  << "   actID"       << setw(11) << act
         << "   TRA_ctrl_packet generating"
         << endl;
}

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

class SDN_ctrl_pkt_gen_event: public event {
    public:
        class gen_data; // forward declaration
            
    private:
        SDN_ctrl_pkt_gen_event (SDN_ctrl_pkt_gen_event &){}
        SDN_ctrl_pkt_gen_event (){} // we don't allow users to new a recv_event by themselves
        // this constructor cannot be directly called by users; only by generator
        unsigned int src; // the src
        unsigned int dst; // the dst 
        unsigned int mat;
        unsigned int act;
        // packet *pkt; // the packet
        string msg;
        double per; // percentage
    
    protected:
        SDN_ctrl_pkt_gen_event (unsigned int _trigger_time, void *data): event(_trigger_time), src(BROCAST_ID), dst(BROCAST_ID){
            pkt_gen_data * data_ptr = (pkt_gen_data*) data;
            src = data_ptr->src_id;
            dst = data_ptr->dst_id;
            // pkt = data_ptr->_pkt;
            mat = data_ptr->mat_id;
            act = data_ptr->act_id;
            msg = data_ptr->msg;
            per = data_ptr->per;
        } 
        
    public:
        virtual ~SDN_ctrl_pkt_gen_event(){}
        // SDN_ctrl_pkt_gen_event will trigger the packet gen function
        virtual void trigger();
        
        unsigned int event_priority() const;
        
        class SDN_ctrl_pkt_gen_event_generator;
        friend class SDN_ctrl_pkt_gen_event_generator;
        // SDN_ctrl_pkt_gen_event_generator is derived from event_generator to generate an event
        class SDN_ctrl_pkt_gen_event_generator : public event_generator{
                static SDN_ctrl_pkt_gen_event_generator sample;
                // this constructor is only for sample to register this event type
                SDN_ctrl_pkt_gen_event_generator() { /*cout << "send_event registered" << endl;*/ register_event_type(&sample); }
            protected:
                virtual event * generate(unsigned int _trigger_time, void *data){ 
                    // cout << "send_event generated" << endl; 
                    return new SDN_ctrl_pkt_gen_event(_trigger_time, data); 
                }
            
            public:
                virtual string type() { return "SDN_ctrl_pkt_gen_event";}
                ~SDN_ctrl_pkt_gen_event_generator(){}
        };
        // this class is used to initialize the SDN_ctrl_pkt_gen_event
        class pkt_gen_data{
            public:
                unsigned int src_id; // the controller
                unsigned int dst_id; // the node that should update its rule
                unsigned int mat_id; // the target of the rule
                unsigned int act_id; // the next hop toward the target recorded in the rule
                string msg;
                double per; // the percentage
                // packet *_pkt;
        };
        
        void print () const;
};
SDN_ctrl_pkt_gen_event::SDN_ctrl_pkt_gen_event_generator SDN_ctrl_pkt_gen_event::SDN_ctrl_pkt_gen_event_generator::sample;

void SDN_ctrl_pkt_gen_event::trigger() {
    if ( dst == BROCAST_ID || node::id_to_node(dst) == nullptr ) {
        cerr << "SDN_ctrl_pkt_gen_event error: no node " << dst << "!" << endl;
        return;
    }
    
    SDN_ctrl_packet *pkt = dynamic_cast<SDN_ctrl_packet*> ( packet::packet_generator::generate("SDN_ctrl_packet") );
    if (pkt == nullptr) { 
        cerr << "packet type is incorrect" << endl; return; 
    }
    SDN_ctrl_header *hdr = dynamic_cast<SDN_ctrl_header*> ( pkt->getHeader() );
    SDN_ctrl_payload *pld = dynamic_cast<SDN_ctrl_payload*> ( pkt->getPayload() );
    
    if (hdr == nullptr) {
        cerr << "header type is incorrect" << endl; return ;
    }
    if (pld == nullptr) {
        cerr << "payload type is incorrect" << endl; return ;
    }

    hdr->setSrcID(src); 
    hdr->setDstID(dst);
    hdr->setPreID(src);
    // hdr->setNexID(dst); // in hw3, you should set NexID to src
    hdr->setNexID(src);
    
    // payload
    pld->setMsg(msg);
    pld->setMatID(mat);
    pld->setActID(act);
    pld->setPer(per);
    
    recv_event::recv_data e_data;
    e_data.s_id = src;
    // e_data.r_id = dst; // in hw3, you should set r_id it src
    e_data.r_id = src;
    e_data._pkt = pkt;
    
    recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",trigger_time, (void *)&e_data) );
}
unsigned int SDN_ctrl_pkt_gen_event::event_priority() const {
    string string_for_hash;
    string_for_hash = to_string(getTriggerTime()) + to_string(src) + to_string(dst) + to_string(mat) + to_string(act); //to_string (pkt->getPacketID());
    return get_hash_value(string_for_hash);
}
// the SDN_ctrl_pkt_gen_event::print() function is used for log file
void SDN_ctrl_pkt_gen_event::print () const {
    cout << "time "          << setw(11) << event::getCurTime() 
         << "        "       << setw(11) << " "
         << " percent"       << setw(11) << per
         << "   srcID"       << setw(11) << src 
         << "   dstID"       << setw(11) << dst
         << "   matID"       << setw(11) << mat
         << "   actID"       << setw(11) << act
         << "   SDN_ctrl_packet generating"
         << endl;
}


////////////////////////////////////////////////////////////////////////////////

class link {
        // all links created in the program
        static map< pair<unsigned int,unsigned int>, link*> id_id_link_table;
        unsigned int id1; // from
        unsigned int id2; // to
        
    protected:
        link(link&){} // this constructor should not be used
        link(){} // this constructor should not be used
        link(unsigned int _id1, unsigned int _id2): id1(_id1), id2(_id2) { id_id_link_table[pair<unsigned int,unsigned int>(id1,id2)] = this; }

    public:
        virtual ~link() { 
            id_id_link_table.erase (pair<unsigned int,unsigned int>(id1,id2)); // erase the link
        }
        
        static link * id_id_to_link (unsigned int _id1, unsigned int _id2) { 
            return ((id_id_link_table.find(pair<unsigned int,unsigned int>(_id1,_id2))!=id_id_link_table.end()) ? id_id_link_table[pair<unsigned,unsigned>(_id1,_id2)]: nullptr) ; 
        }

        virtual double getLatency() = 0; // you must implement your own latency
        
        static void del_link (unsigned int _id1, unsigned int _id2) {
            pair<unsigned int, unsigned int> temp;
            if (id_id_link_table.find(temp)!=id_id_link_table.end())
                id_id_link_table.erase(temp); 
        }

        static unsigned int getLinkNum () { return id_id_link_table.size(); }

        class link_generator {
                // lock the copy constructor
                link_generator(link_generator &){}
                // store all possible types of link
                static map<string,link_generator*> prototypes;
            protected:
                // allow derived class to use it
                link_generator() {}
                // after you create a new link type, please register the factory of this link type by this function
                void register_link_type(link_generator *h) { prototypes[h->type()] = h; }
                // you have to implement your own generate() to generate your link
                virtual link* generate(unsigned int _id1, unsigned int _id2) = 0;
            public:
                // you have to implement your own type() to return your link type
        	    virtual string type() = 0;
        	    // this function is used to generate any type of link derived
        	    static link * generate (string type, unsigned int _id1, unsigned int _id2) {
        	        if(id_id_link_table.find(pair<unsigned int,unsigned int>(_id1,_id2))!=id_id_link_table.end()){
        	            std::cerr << "duplicate link id" << std::endl; // link id is duplicated
        	            return nullptr;
        	        }
        	        else if ( BROCAST_ID == _id1 || BROCAST_ID == _id2 ) {
        	            cerr << "BROCAST_ID cannot be used" << endl;
        	            return nullptr;
        	        }
            		else if (prototypes.find(type) != prototypes.end()){ // if this type derived exists 
            		    link * created_link = prototypes[type]->generate(_id1,_id2);
            			return created_link; // generate it!!
            		}
            		std::cerr << "no such link type" << std::endl; // otherwise
            		return nullptr;
            	}
            	static void print () {
            	    cout << "registered link types: " << endl;
            	    for (map<string,link::link_generator*>::iterator it = prototypes.begin(); it != prototypes.end(); it ++)
            	        cout << it->second->type() << endl;
            	}
            	virtual ~link_generator(){};
        };
};
map<string,link::link_generator*> link::link_generator::prototypes;
map<pair<unsigned int,unsigned int>, link*> link::id_id_link_table;

void node::add_phy_neighbor (unsigned int _id, string link_type){
    if (id == _id) return; // if the two nodes are the same...
    if (id_node_table.find(_id)==id_node_table.end()) return; // if this node does not exist
    if (phy_neighbors.find(_id)!=phy_neighbors.end()) return; // if this neighbor has been added
    phy_neighbors[_id] = true;
    
    link::link_generator::generate(link_type,id,_id);
}
void node::del_phy_neighbor (unsigned int _id){
    phy_neighbors.erase(_id);
    
}


class simple_link: public link {
    protected:
        simple_link() {} // it should not be used outside the class
        simple_link(simple_link&) {} // it should not be used
        simple_link(unsigned int _id1, unsigned int _id2): link (_id1,_id2){} // this constructor cannot be directly called by users
    
    public:
        virtual ~simple_link() {}
        virtual double getLatency() { return ONE_HOP_DELAY; } // you can implement your own latency
        
        class simple_link_generator;
        friend class simple_link_generator;
        // simple_link is derived from link_generator to generate a link
        class simple_link_generator : public link_generator {
                static simple_link_generator sample;
                // this constructor is only for sample to register this link type
                simple_link_generator() { /*cout << "simple_link registered" << endl;*/ register_link_type(&sample); }
            protected:
                virtual link * generate(unsigned int _id1, unsigned int _id2) 
                { /*cout << "simple_link generated" << endl;*/ return new simple_link(_id1,_id2); }
            public:
                virtual string type() { return "simple_link"; }
                ~simple_link_generator(){}
        };
};

simple_link::simple_link_generator simple_link::simple_link_generator::sample;


// the data_packet_event function is used to add an initial event
void data_packet_event (unsigned int src, unsigned int dst, unsigned int t = 0, string msg = "default"){
    if ( node::id_to_node(src) == nullptr || (dst != BROCAST_ID && node::id_to_node(dst) == nullptr) ) {
        cerr << "src or dst is incorrect" << endl; return ;
        return;
    }

    TRA_data_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = src;
    e_data.dst_id = dst;
    e_data.msg = msg;
    
    // recv_event *e = dynamic_cast<recv_event*> ( event::event_generator::generate("recv_event",t, (void *)&e_data) );
    TRA_data_pkt_gen_event *e = dynamic_cast<TRA_data_pkt_gen_event*> ( event::event_generator::generate("TRA_data_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// the TRA_ctrl_packet_event function is used to add an initial event
void TRA_ctrl_packet_event (unsigned int src, unsigned int t = event::getCurTime(),
                        string msg = "default") {
        // 1st parameter: the source; the destination that want to broadcast a msg with counter 0 (i.e., match ID)
        // 2nd parameter: time (optional)
        // 3rd parameter: msg for debug information (optional)
    if ( node::id_to_node(src) == nullptr ) {
        cerr << "id is incorrect" << endl; return;
    }
    
    // unsigned int src = con_id;
    TRA_ctrl_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = src;
    e_data.dst_id = BROCAST_ID;
    // e_data.mat_id = mat;
    // e_data.act_id = act;
    e_data.msg = msg;
    // e_data.per = per;
    
    TRA_ctrl_pkt_gen_event *e = dynamic_cast<TRA_ctrl_pkt_gen_event*> ( event::event_generator::generate("TRA_ctrl_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// the SDN_ctrl_packet_event function is used to add an initial event
void SDN_ctrl_packet_event (unsigned int con_id, unsigned int id, 
                        unsigned int mat, unsigned int act, 
                        double per, unsigned int t = event::getCurTime(),
                        string msg = "default") {
        // 1st parameter: the controller id
        // 2nd parameter: the id of node that has to update the table
        // 3rd parameter: the destination id (i.e., match ID)
        // 4th parameter: the next-hop node toward the target node recorded in the rule (i.e., action ID)
        // 5th parameter: the percentage (e.g., 45%: 0.45)
        // 6th parameter: time (optional)
        // 7th parameter: msg for debug information (optional)
    // cerr << "con_id: " << con_id << endl;
    if ( id == BROCAST_ID || node::id_to_node(id) == nullptr ) {
        cerr << "id is incorrect" << endl; return;
    }
    
    unsigned int src = con_id;
    SDN_ctrl_pkt_gen_event::pkt_gen_data e_data;
    e_data.src_id = src;
    e_data.dst_id = id;
    e_data.mat_id = mat;
    e_data.act_id = act;
    e_data.msg = msg;
    e_data.per = per;
    
    SDN_ctrl_pkt_gen_event *e = dynamic_cast<SDN_ctrl_pkt_gen_event*> ( event::event_generator::generate("SDN_ctrl_pkt_gen_event",t, (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

// send_handler function is used to transmit packet p based on the information in the header
// Note that the packet p will not be discard after send_handler ()

void node::send_handler(packet *p){
    packet *_p = packet::packet_generator::replicate(p);
    send_event::send_data e_data;
    e_data.s_id = _p->getHeader()->getPreID();
    e_data.r_id = _p->getHeader()->getNexID();
    e_data._pkt = _p;
    send_event *e = dynamic_cast<send_event*> (event::event_generator::generate("send_event",event::getCurTime(), (void *)&e_data) );
    if (e == nullptr) cerr << "event type is incorrect" << endl;
}

void node::send(packet *p){ // this function is called by event; not for the user
    if (p == nullptr) return;
    
    unsigned int _nexID = p->getHeader()->getNexID();
    for ( map<unsigned int,bool>::iterator it = phy_neighbors.begin(); it != phy_neighbors.end(); it ++) {
        unsigned int nb_id = it->first; // neighbor id
        
        if (nb_id != _nexID && BROCAST_ID != _nexID) continue; // this neighbor will not receive the packet
        
        unsigned int trigger_time = event::getCurTime() + link::id_id_to_link(id, nb_id)->getLatency() ; // we simply assume that the delay is fixed
        // cout << "node " << id << " send to node " <<  nb_id << endl;
        recv_event::recv_data e_data;
        e_data.s_id = id;    // set the sender   (i.e., preID)
        e_data.r_id = nb_id; // set the receiver (i.e., nexID)
        
        packet *p2 = packet::packet_generator::replicate(p);
        e_data._pkt = p2;
        
        recv_event *e = dynamic_cast<recv_event*> (event::event_generator::generate("recv_event", trigger_time, (void*) &e_data)); // send the packet to the neighbor
        if (e == nullptr) cerr << "event type is incorrect" << endl;
    }
    packet::discard(p);
}

// you have to write the code in recv_handler of TRA_switch
void TRA_switch::recv_handler (packet *p){
  // in this function, you are "not" allowed to use node::id_to_node(id) !!!!!!!!

  // this is a simple example
  // node 0 broadcasts its message to every node and every node relays the packet "only once" and increases its counter
  // the variable hi is used to examine whether the packet has been received by this node before
  // you can remove the variable hi and create your own routing table in class TRA_switch
  if (p == nullptr) return ;
  
  if (p->type() == "TRA_ctrl_packet") { // the switch receives a packet from the controller
      TRA_ctrl_packet *p3 = nullptr;
      p3 = dynamic_cast<TRA_ctrl_packet*> (p);
      TRA_ctrl_payload *l3 = nullptr;
      l3 = dynamic_cast<TRA_ctrl_payload*> (p3->getPayload());
      // cerr << p3 -> getHeader() -> getSrcID() << '\n';
      if(recieveTime.find(p3 -> getHeader() -> getSrcID()) == recieveTime.end()) recieveTime[p3 -> getHeader() -> getSrcID()] = event::getCurTime();
      if(recieveTime[p3 -> getHeader() -> getSrcID()] < event::getCurTime()){
        // cout << 1 ;
        // cout<<"this node " << getNodeID() << ' ' << "now time " << event::getCurTime() << ' ' << endl;
        // cout<<"prenode " << p3 -> getHeader() -> getPreID() << ' ' << "routingTable " << routingTable[p3 -> getHeader() -> getSrcID()] << ' ' << endl;
        return;
      }
      if(routingTable.find(p3 -> getHeader() -> getSrcID()) != routingTable.end() && p3 -> getHeader() -> getPreID() >= routingTable[p3 -> getHeader() -> getSrcID()]){
        // cout << 2;
        // cout<<"this node " << getNodeID() << ' ' << "now time " << event::getCurTime() << ' ' << endl;
        // cout<<"prenode " << p3 -> getHeader() -> getPreID() << ' ' << "routingTable " << routingTable[p3 -> getHeader() -> getSrcID()] << ' ' << endl;
        return;
      }
      // cout<<"this node " << getNodeID() << ' ' << "now time " << event::getCurTime() << ' ' << endl;
      addNextNode(p3->getHeader()->getSrcID(), p3->getHeader()->getPreID());// add the next node
      // cout << "node " << getNodeID() << " receives a packet from the controller" << endl;
      // cout << "node " << getNodeID() << " broadcasts the packet" << endl;
      // cout << "srcnode " << p3->getHeader()->getSrcID() << " sends the packet to the controller" << endl;
      // cout << "preID " << p3->getHeader()->getPreID() << " sends the packet to the controller" << endl;
      p3->getHeader()->setPreID ( getNodeID() );
      p3->getHeader()->setNexID ( BROCAST_ID );
      p3->getHeader()->setDstID ( BROCAST_ID );
      l3->increase();
      send_handler(p3);
      // unsigned mat = l3->getMatID();
      // unsigned act = l3->getActID();
      // string msg = l3->getMsg(); // get the msg
  }
  else if (p->type() == "TRA_data_packet" ) { // the switch receives a packet
      // cout << "node " << getNodeID() << " send the packet" << endl;
      TRA_data_packet *p3 = nullptr;
      p3 = dynamic_cast<TRA_data_packet*> (p);
      TRA_data_payload *l3 = nullptr;
      l3 = dynamic_cast<TRA_data_payload*> (p3->getPayload());
      if(routingTable.find(p3->getHeader()->getDstID()) == routingTable.end()) return;
      p3->getHeader()->setPreID ( getNodeID() );
      p3->getHeader()->setNexID ( routingTable[p3->getHeader()->getDstID()] );
      p3->getHeader()->setDstID ( p3->getHeader()->getDstID() );
      if(getNodeID() != p3->getHeader()->getDstID()) send_handler(p3);
  }

  // you should implement the OSPF algorithm in recv_handler
  // getNodeID() returns the id of the current node
  
  // The current node's neighbors are already stored in the following variable 
  // map<unsigned int,bool> node::phy_neighbors
  // however, this variable is private in the class node
  // You have to use node::getPhyNeighbors to get the variable
  // for example, if you want to print all the neighbors of this node
  // const map<unsigned int,bool> &nblist = getPhyNeighbors();
  // cout << "node " << getNodeID() << "'s nblist: ";
  // for (map<unsigned int,bool>::const_iterator it = nblist.begin(); it != nblist.end(); it ++) {
  //     cout << it->first << ", " ;
  // }
  // cout << endl;


  // you can use p->getHeader()->setSrcID() or getSrcID()
  //             p->getHeader()->setDstID() or getDstID()
  //             p->getHeader()->setPreID() or getPreID()
  //             p->getHeader()->setNexID() or getNexID() to change or read the packet header
  
  // In addition, you can get the packet, header, and payload with the correct type (GR)
  // in fact, this is downcasting
  // TRA_data_packet * pkt = dynamic_cast<TRA_data_packet*> (p);
  // TRA_data_header * hdr = dynamic_cast<TRA_data_header*> (p->getHeader());
  // TRA_data_payload * pld = dynamic_cast<TRA_data_payload*> (p->getPayload());
  
  // you can also change the TRA_data_payload setting
  // pld->setMsg(string): to set the message transmitted to the destination
  
  // Besides, you can use packet::packet_generator::generate() to generate a new packet; note that you should fill the header and payload in the packet
  // moreover, you can use "packet *p2 = packet::packet_generator::replicate(p)" to make a clone p2 of packet p
  // note that if the packet is generated or replicated manually, you must delete it by packet::discard() manually before recv_handler finishes
  
  // "IMPORTANT":
  // You have to "carefully" fill the correct information (e.g., srcID, dstID, ...) in the packet before you send it
  // Note that if you want to transmit a packet to only one next node (i.e., unicast), then you fill the ID of a specific node to "nexID" in the header
  // Otherwise, i.e., you want to broadcasts, then you fill "BROCAST_ID" to "nexID" in the header
  // after that, you can use send() to transmit the packet 
  // usage: send_handler (p);
  
  // note that packet p will be discarded (deleted) after recv_handler(); you don't need to manually delete it
}




///////////////////////////////////////////////////////////////////////////////////////////////


// you have to write the code in recv_handler of SDN_switch
void SDN_switch::recv_handler (packet *p){
    if (p == nullptr) return ;
    
    if (p->type() == "TRA_ctrl_packet" /*&& !hi*/) { // the switch receives a packet from the controller
        TRA_ctrl_packet *p3 = nullptr;
      p3 = dynamic_cast<TRA_ctrl_packet*> (p);
      TRA_ctrl_payload *l3 = nullptr;
      l3 = dynamic_cast<TRA_ctrl_payload*> (p3->getPayload());
      if(recieveTime.find(p3 -> getHeader() -> getSrcID()) == recieveTime.end()) recieveTime[p3 -> getHeader() -> getSrcID()] = event::getCurTime();
      if(recieveTime[p3 -> getHeader() -> getSrcID()] < event::getCurTime()){
        // cout << 1 ;
        // cout<<"this node " << getNodeID() << ' ' << "now time " << event::getCurTime() << ' ' << endl;
        // cout<<"prenode " << p3 -> getHeader() -> getPreID() << ' ' << "routingTable " << routingTable[p3 -> getHeader() -> getSrcID()] << ' ' << endl;
        return;
      }
      if(routingTable.find(p3 -> getHeader() -> getSrcID()) != routingTable.end() && p3 -> getHeader() -> getPreID() >= routingTable[p3 -> getHeader() -> getSrcID()]){
        // cout << 2;
        // cout<<"this node " << getNodeID() << ' ' << "now time " << event::getCurTime() << ' ' << endl;
        // cout<<"prenode " << p3 -> getHeader() -> getPreID() << ' ' << "routingTable " << routingTable[p3 -> getHeader() -> getSrcID()] << ' ' << endl;
        return;
      }
      // cout<<"this node " << getNodeID() << ' ' << "now time " << event::getCurTime() << ' ' << endl;
      addNextNode(p3->getHeader()->getSrcID(), p3->getHeader()->getPreID());// add the next node
      // cout << "node " << getNodeID() << " receives a packet from the controller" << endl;
      // cout << "node " << getNodeID() << " broadcasts the packet" << endl;
      // cout << "srcnode " << p3->getHeader()->getSrcID() << " sends the packet to the controller" << endl;
      // cout << "preID " << p3->getHeader()->getPreID() << " sends the packet to the controller" << endl;
      p3->getHeader()->setPreID ( getNodeID() );
      p3->getHeader()->setNexID ( BROCAST_ID );
      p3->getHeader()->setDstID ( BROCAST_ID );
      l3->increase();
      send_handler(p3);
      // unsigned mat = l3->getMatID();
      // unsigned act = l3->getActID();
      // string msg = l3->getMsg(); // get the msg
    }
    else if (p->type() == "TRA_data_packet" ) { // the switch receives a packet
        //this->tra_data_Handler->handle(this, packet);
    }else if(p->type() == "SDN_ctrl_packet"){
        //cout << this->getNodeID() << " receive SDN ctrl packet" << endl;
        SDN_ctrl_packet* ctrlPacket = dynamic_cast<SDN_ctrl_packet*>(p);
        SDN_ctrl_header* header = dynamic_cast<SDN_ctrl_header*>(ctrlPacket->getHeader());
        SDN_ctrl_payload* payload = dynamic_cast<SDN_ctrl_payload*>(ctrlPacket->getPayload());

        int mat = payload->getMatID();
        int act = payload->getActID();
        double per = payload->getPer();
        // cerr << "mat: " << mat << " act: " << act << " per: " << per << endl;
        table[mat].push_back({act, per});
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////


// you have to write the code in recv_handler of SDN_switch
void SDN_controller::recv_handler (packet *p){
    if (p == nullptr) return ;
    
    if (p->type() == "TRA_ctrl_packet" /*&& !hi*/) { // the switch receives a packet from the controller
        return;
    }
    else if (p->type() == "TRA_data_packet" ) { // the switch receives a packet
        return;
    }else if(p->type() == "SDN_ctrl_packet"){
        //cout << this->getNodeID() << " receive SDN ctrl packet" << endl;
        SDN_ctrl_packet* ctrlPacket = dynamic_cast<SDN_ctrl_packet*>(p);
        SDN_ctrl_header* header = dynamic_cast<SDN_ctrl_header*>(ctrlPacket->getHeader());
        SDN_ctrl_payload* payload = dynamic_cast<SDN_ctrl_payload*>(ctrlPacket->getPayload());
        
        header->setNexID(header->getDstID());
        // cerr << "src: " << header->getSrcID() << " dst: " << header->getDstID() << endl;
        send_handler(ctrlPacket);

    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////




class my_node
{
private:
	// type用來記錄這個節點是否為SDN節點，true代表為SDN節點。
	bool type;
public:
	int packet;
	map<unsigned int, vector<pair<unsigned int, double>>> save_flow;
	my_node() :packet(0), type(false) {}
	bool is_SDN() {
		return type == true;
	}
	void change_type() {
		type = !type;
	}
};

class Edge {
public:
	int from;
	int to;
	int cap;
	int rev;
	Edge() {}
	Edge(int _from, int _to, int _cap, int _rev) :from(_from), to(_to), cap(_cap), rev(_rev) {}
};

vector<vector<int>> bfs(int dst, vector<vector<int>> &graph, int node_num) {
	queue<int> save;
	vector<bool> visited(node_num, false);
	vector<vector<int>> ans(node_num);
	save.push(dst);
	visited[dst] = true;
	int level = 0;
	while (!save.empty()) {
		int len = save.size();
        vector<int> temp;
        for(int i = 0; i < len; i++){
            int n = save.front();
            save.pop();
            temp.push_back(n);
        }
        sort(temp.begin(), temp.end());
		for (int i = 0; i < len; i++) {
			int n = temp[i];
			ans[level].push_back(n);
			for (int &nnn : graph[n]) {
				if (!visited[nnn]) {
					visited[nnn] = true;
					save.push(nnn);
				}
			}
		}
		level++;
	}
	return ans;
}

void output(vector<my_node> &n, vector<int> &dst, int node_num) {
	for (int i = 0; i < node_num; i++) {
		cout << i << '\n';
		if (n[i].is_SDN()) {
			for (int j = 0; j < dst.size(); j++) {
				if (dst[j] == i) {
					cout << i << ' ' << dst[j] << " 100%\n";
					continue;
				}
				cout << dst[j] << ' ';
				for (auto &k : n[i].save_flow[dst[j]]) {
					if(k.second == 0) continue;
					cout << k.first << ' ' << k.second * 100.0 << "% ";
				}
				cout << '\n';
			}
		}
		else {
			for (int j = 0; j < dst.size(); j++) {
				if (dst[j] == i) {
					cout << i << ' ' << dst[j] << '\n';
					continue;
				}
				cout << dst[j] << ' ' << n[i].save_flow[dst[j]][0].first << "\n";
			}
		}
	}
}

// 有優化速度的空間
/*vector<vector<int>> Create_DAG(int node_num, vector<vector<int>> &level, vector<my_node> &n, vector<vector<bool>> &graph) {
	vector<vector<int>> DAG(node_num);
	for (int i = node_num - 1; i >= 1; i--) {
		if (level[i].size() == 0) continue;
		// 便歷那一層的節點數
		for (int j = 0; j < level[i].size(); j++) {
			for (int k = 0; k < level[i - 1].size(); k++) {
				if (graph[level[i][j]][level[i - 1][k]] == true) {
					DAG[level[i][j]].push_back(level[i - 1][k]);
					if (!n[level[i][j]].is_SDN()) break;
				}
			}
		}
	}
	return DAG;
}*/

// bool have_cycle(vector<vector<int>> &DAG, int src, int dst){
//     // 起點到終點
//   if(src == dst) return true;
//   for(int i = 0; i < DAG[src].size(); i++){
//     if(have_cycle(DAG, DAG[src][i], dst)) return true;
//   }
//   return false;
// }

class judge{
    static long long count;
    static clock_t start;
    public:
    clock_t starttime(){
        return start;
    }
    bool have_cycle(vector<vector<int>> &DAG, int src, int dst){
        count++;
        if(count > 50) return true;
        // cerr << "count "<<count << ' ' <<src << ' ' << dst <<  '\n';
        // 起點到終點
        queue<int> save;
        save.push(src);
        while(!save.empty()){
            int len = save.size();
            for(int i = 0; i < len; i++){
                int node = save.front();
                save.pop();
                if(node == dst) return true;
                for(auto &j: DAG[node]){
                    save.push(j);
                }
            }
        }
        return false;
    }
    long long num(){
        return count;
    }
};
long long judge::count = 0;
clock_t judge::start = clock();

vector<vector<int>> Create_DAG(vector<vector<int>> &graph, int node_num, int dst, vector<my_node> &n){
    judge j;
    auto level = bfs(dst, graph, node_num);
    map<int, int> level_map;
    for(int i = 0; i < level.size(); i++){
        for(auto &j: level[i]){
            level_map[j] = i;
        }
    }
  vector<vector<int>> DAG(node_num);
  queue<int> save;
  save.push(dst);
  vector<bool> visit(node_num, false);
 
  // 確認有沒有被其他點插入過
  vector<vector<bool>> check(node_num, vector<bool>(node_num, false));
  while (!save.empty())
  { 
    int len = save.size();
    vector<int> temp;
    for(int k = 0; k < len; k++){
        int x = save.front();
        save.pop();
        temp.push_back(x);
    }
    sort(temp.begin(), temp.end());
    for(int k = 0; k < len; k++){
        int node = temp[k];
        // node 是被插的
        for(auto &i:graph[node]){
          // cerr << graph.size() <<' ' <<node << ' ' << i <<'\n';
            // 代表i點沒被node點插過
            // 所以i點可以插入node點
            if(check[i][node]) continue;
            if(check[node][i]) continue;
            if(n[i].is_SDN()){
                // assert(level[level_map[i]].size() < 0.8 * node_num);
                // clock_t end = clock();
                // double cpu_time_used = ((double) (end - j.starttime())) / CLOCKS_PER_SEC;
                // cerr << cpu_time_used << '\n';
                if(level_map[i] == level_map[node]){
                    if(j.have_cycle(DAG, node, i)){
                        continue;
                    }
                }
                DAG[i].push_back(node);
                // node點被i點插過
                check[node][i] = true;
            }else{
                // 如果一般點已經插入過別人，continue
                if(visit[i]) continue;
                DAG[i].push_back(node);
                visit[i] = true;
                check[node][i] = true;
                // 關於一般點要不要給很多點插入，這個作保留來實驗。
            }
            save.push(i);
        }
    }
  }
  return DAG;
}

bool vis[600];

void add_edge(int cap, int from, int to, vector<vector<Edge>> &G) {
	G[from].push_back(Edge(from, to, cap, G[to].size()));
	G[to].push_back(Edge(to, from, 0, G[from].size() - 1));
}

int dfs(int now, int target, int flow, vector<vector<Edge>> &G, vector<my_node> &n) {
	if (now == target) return flow;
	vis[now] = true;
	for (int i = 0; i < G[now].size(); i++) {
		Edge &e = G[now][i];
		if (!vis[e.to] && e.cap > 0) {
			int d = dfs(e.to, target, min(flow, e.cap), G, n);
			if (d > 0) {
				e.cap -= d;
				G[e.to][e.rev].cap += d;
				return d;
			}
		}
	}
	return 0;
}

int max_flow(int s, int t, int flow, vector<vector<Edge>> &G, vector<my_node> &n) {
	int r = 0;
	while (1) {
		memset(vis, 0, sizeof(vis));
		int f = dfs(s, t, flow, G, n);
		if (f <= 0) break;;
		r += f;
	}
	return r;
}

void save_node_packet(vector<my_node> &n, vector<vector<int>> &DAG, vector<vector<int>> &adj_m, vector<pair<int, int>> &s){
	int node_num = n.size() - 1;
	for(int i = 0; i < node_num; i++){
		for(auto &j: DAG[i]){
			n[i].packet += adj_m[i][j];
		}
	}

}

bool judge_max_flow(int packet, vector<my_node> &n, vector<pair<int, int>> &source, vector<vector<int>> &DAG, int dst, vector<vector<Edge>> &G) {
	// 建立一個新的點，一開始將所有packet匯集在這裡流出去
	// 這樣可以處理多源點問題
	for (auto &i : n)
		i.packet = 0;
	int node_num = n.size() - 1;
	int total = accumulate(source.begin(), source.end(), 0, [](int a, pair<int, int> b) {return a + b.second; });
	n.back().packet = total;
	for (int i = 0; i < source.size(); i++) {
		add_edge(source[i].second, node_num, source[i].first, G);
	}
	// 將無向圖加入流的邊
	for (int i = 0; i < node_num; i++) {
		for (int j = 0; j < DAG[i].size(); j++) {
			add_edge(packet, i, DAG[i][j], G);
		}
	}
	if (max_flow(node_num, dst, total, G, n) == total){
		return true;
	}
	return false;
}





void func(vector<int> dst, vector<vector<int>> graph, int node_num, vector<vector<bool>> graph_matrix, vector<my_node> &n, map<int, vector<pair<int, int>>> save_source){
    int Dst_num = dst.size();
    for (int i = 0; i < Dst_num; i++) {
		auto temp = bfs(dst[i], graph, node_num);
		auto DAG = Create_DAG(graph, node_num, dst[i], n);
		vector<vector<Edge>> G;
		int flow_size;
		int left = 0, right = 2000;
		vector<int> table(10001, -1);
		G.resize(node_num + 1);
		while(left <= right){
			int mid = (left + right) / 2;
			if(table[mid - 1] == -1){
				G.resize(node_num + 1);
				table[mid - 1] = judge_max_flow(mid - 1, n, save_source[dst[i]], DAG, dst[i], G) ? 1 : 0;
				G.clear();
			}
			if(table[mid] == -1){
				G.resize(node_num + 1);
				table[mid] = judge_max_flow(mid, n, save_source[dst[i]], DAG, dst[i], G) ? 1 : 0;
				G.clear();
			}
			if(table[mid] == true && table[mid - 1] == false){
				flow_size = mid;
				break;
			}
			table[mid] == true ? right = mid - 1 : left = mid + 1;
		}
		G.clear();
		G.resize(node_num + 1);
		judge_max_flow(flow_size, n, save_source[dst[i]], DAG, dst[i], G);
		vector<vector<int>> adj_m(node_num, vector<int>(node_num, 0));
		for (int j = 0; j < node_num; j++) {
			for (int k = 0; k < G[j].size(); k++) {
				if (G[j][k].to >= node_num or G[j][k].from >= node_num) continue;
				adj_m[G[j][k].from][G[j][k].to] = flow_size - G[j][k].cap;
			}
		}
		save_node_packet(n, DAG, adj_m, save_source[dst[i]]);
		for (int j = 0; j < node_num; j++) {
			double sum = (double)n[j].packet;
			if (sum == 0) {
				sum = DAG[j].size();
				for (auto &k : DAG[j]) {
					n[j].save_flow[dst[i]].push_back({ k, 1.0 / sum });
				}
			}
			else {
				for (auto &k : DAG[j]) {
                    // cerr << "pay\n";
					n[j].save_flow[dst[i]].push_back({ k, adj_m[j][k] / sum });
				}
			}
		}
	}
}






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    // header::header_generator::print(); // print all registered headers
    // payload::payload_generator::print(); // print all registered payloads
    // packet::packet_generator::print(); // print all registered packets
    // node::node_generator::print(); // print all registered nodes
    // event::event_generator::print(); // print all registered events
    // link::link_generator::print(); // print all registered links 
    
    // read the input and generate switch nodes

    unsigned int node_num, dst_num, link_num, pair_num, sim_time, sdn_ctrl_time, budget;

    cin >> node_num >> dst_num >> link_num >> pair_num >> sim_time >> sdn_ctrl_time >> budget;

    map<int, bool> dst_list;
    vector<int> dl;
    map<int, int> dst_time;

    for(int i = 0; i < dst_num; i++){
      int dst;
      cin >> dst;
      dl.push_back(dst);
      dst_list[dst] = true;
    }
    sort(dl.begin(), dl.end());
    vector<pair<int, int>> price;
    for(int i = 0; i < node_num; i++){
      int node_id, cost, time;
      cin >> node_id >> cost;
      price.push_back(make_pair(cost, node_id));
      if(dst_list.find(i) != dst_list.end()){
        cin >> time;
        dst_time[node_id] = time;
      }
      else{
        // int node_id, cost;
        // cin >> node_id >> cost;
      }
    }
    sort(price.begin(), price.end());
    vector<my_node> n(node_num + 1);
    int c = 0;
    for(int i = 0; i < node_num; i++){
      if(c + price[i].first <= budget){
        // cout << price[i].second << ' ';
        n[price[i].second].change_type();
        c += price[i].first;
      }
      else{
        break;
      }
    }
    for (unsigned int id = 0; id < node_num; id ++){
        
        // if this node is SDN_switch, then create SDN_switch; please define SDN_switch 
        if(n[id].is_SDN()){
          node::node_generator::generate("SDN_switch",id);
        }
        // otherwise, create TRA_switch
        else node::node_generator::generate("TRA_switch",id);
    }
    
    // please write your "SDN_controller: public node" and generate a controller via generate function here
    unsigned int con_id = node::getNodeNum(); // controller id is the last one
    node::node_generator::generate("SDN_controller",con_id);
    //
    ////////
    vector<vector<int>> graph(node_num);
    vector<vector<bool>> graph_matrix(node_num, vector<bool>(node_num, false));
    for(unsigned int i = 0; i < link_num; i++){
      unsigned int edge_id, id1, id2;
      cin >> edge_id >> id1 >> id2;
      graph[id1].push_back(id2);
      graph[id2].push_back(id1);
        graph_matrix[id1][id2] = true;
        graph_matrix[id2][id1] = true;
      node::id_to_node(id1)->add_phy_neighbor(id2);
      node::id_to_node(id2)->add_phy_neighbor(id1);
    }
    for(int i = 0; i < node_num; i++){
        sort(graph[i].begin(), graph[i].end());
    }
    map<int, vector<pair<int, int>>> save_source;
    for(unsigned int i = 0; i < pair_num; i++){
      int id, src, dst, flow;
      cin >> id >> src >> dst >> flow;
        save_source[dst].push_back(make_pair(src, flow));
    }
    func(dl, graph, node_num, graph_matrix, n, save_source);
    for(int i = 0; i < node_num; i++){
        if(n[i].is_SDN()){
            node::id_to_node(con_id)->add_phy_neighbor(i);
            node::id_to_node(i)->add_phy_neighbor(con_id);
            // cerr << i << ' ';
            if(dst_list.find(i) != dst_list.end()) SDN_ctrl_packet_event(con_id, i, i, i, 1, sdn_ctrl_time);
            for(auto t:n[i].save_flow){
                for(int j = 0; j < t.second.size(); j++){
                    // cerr << i << ' ' << t.first << ' ' << t.second[j].first << ' ' << t.second[j].second << '\n';
                    SDN_ctrl_packet_event(con_id, i, t.first, t.second[j].first, t.second[j].second, sdn_ctrl_time);
                }
            }
        }
    }
    // set switches' neighbors
    // node::id_to_node(0)->add_phy_neighbor(1);
    // node::id_to_node(1)->add_phy_neighbor(0);
    // node::id_to_node(0)->add_phy_neighbor(2);
    // node::id_to_node(2)->add_phy_neighbor(0);
    // node::id_to_node(1)->add_phy_neighbor(2);
    // node::id_to_node(2)->add_phy_neighbor(1);
    // node::id_to_node(1)->add_phy_neighbor(3);
    // node::id_to_node(3)->add_phy_neighbor(1);
    // node::id_to_node(2)->add_phy_neighbor(4);
    // node::id_to_node(4)->add_phy_neighbor(2);
    
    // please link every SDN_node to the SDN_controller
    //
    ////////

    // node 0 broadcasts a msg with counter 0 at time 100

    for(auto it = dst_time.begin(); it != dst_time.end(); it++){
      TRA_ctrl_packet_event(it -> first, it -> second);
    }

    
    // 1st parameter: the source; the destination that want to broadcast a msg with counter 0 (i.e., match ID)
    // 2nd parameter: time (optional)
    // 3rd parameter: msg for debug information (optional)
    
    // node with "con_id" (i.e., controller) sends a packet to node 0 at time 150 with matID 3, actID 4, percentage 35%
    // --> you need to implement SDN_switch, SDN_controller, routing tables for SDN_switch
    // SDN_ctrl_packet_event(con_id, 0, 3, 4, 0.35, 150); 
    // 1st parameter: the controller id
    // 2nd parameter: the id of an SDN_switch that has to update the table
    // 3rd parameter: the destination id (i.e., match ID)
    // 4th parameter: the next-hop node toward the target node recorded in the rule (i.e., action ID)
    // 5th parameter: the percentage (e.g., 45%: 0.45)
    // 6th parameter: time (optional)
    // 7th parameter: msg for debug information (optional)

    // node 4 sends a packet to node 0 at time 200
    // --> you need to implement routing tables for TRA_switch
    // data_packet_event(4, 0, 200); // data_packet is type TRA_data_packet
    // 1st parameter: the source node
    // 2nd parameter: the destination node
    // 3rd parameter: time
    // 4th parameter: msg for debug (optional)
    // start simulation!!
    event::start_simulate(sim_time);
    for(int i = 0; i < node_num; i++){
        if(n[i].is_SDN()){
            cout << i << ' ';
        }
    }
    cout << '\n';
    // output(n, dl, node_num);
    node::print_all_routing_table();
    // event::flush_events() ;
    // cout << packet::getLivePacketNum() << endl;
    return 0;
}

