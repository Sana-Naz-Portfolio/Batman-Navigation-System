#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <climits>
#include <algorithm>
#include <iomanip>

using namespace std;

// ================= COLORS & UTILS =================
const string GREEN  = "\033[32m";
const string RED    = "\033[31m";
const string YELLOW = "\033[33m";
const string CYAN   = "\033[36m";
const string RESET  = "\033[0m";

void printSeparator() {
    cout << CYAN << "============================================================" << RESET << endl;
}

void printHeader(string title) {
    printSeparator();
    cout << setw(35) << title << endl;
    printSeparator();
}

// ================= 1. VILLAIN TRACKING (LINKED LIST) =================
struct VillainNode {
    string name;
    string location;
    string crimeDescription;
    string timestamp;
    VillainNode* next;

    VillainNode(string n, string l, string c, string t)
        : name(n), location(l), crimeDescription(c), timestamp(t), next(nullptr) {}
};

class VillainTracker {
private:
    VillainNode* head;

public:
    VillainTracker() : head(nullptr) {}

    void addSighting(string name, string loc, string crime, string time) {
        VillainNode* newNode = new VillainNode(name, loc, crime, time);
        if (!head) {
            head = newNode;
        } else {
            VillainNode* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
        cout << GREEN << "[ALFRED]: Sir, I've updated the database. " << name << " spotted at " << loc << "." << RESET << endl;
    }

    void resolveSighting(string name) {
        if (!head) {
            cout << RED << "[COMPUTER]: No active villain records found." << RESET << endl;
            return;
        }
        if (head->name == name) {
            VillainNode* toDelete = head;
            head = head->next;
            delete toDelete;
            cout << GREEN << "[ALFRED]: Very good, sir. " << name << " has been apprehended. Record archived." << RESET << endl;
            return;
        }
        VillainNode* temp = head;
        while (temp->next && temp->next->name != name)
            temp = temp->next;

        if (temp->next) {
            VillainNode* toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
            cout << GREEN << "[ALFRED]: Target " << name << " neutralized and removed from the tracker." << RESET << endl;
        } else {
            cout << YELLOW << "[COMPUTER]: Villain not found in current active log." << RESET << endl;
        }
    }

    void displayTimeline() {
        printHeader("VILLAIN ACTIVITY TIMELINE");
        if (!head) {
            cout << "No active threats detected. Gotham is quiet... for now." << endl;
            return;
        }
        VillainNode* temp = head;
        while (temp) {
            cout << RED << "TARGET: " << temp->name << RESET << endl;
            cout << "Last Seen: " << temp->location        << endl;
            cout << "Activity:  " << temp->crimeDescription << endl;
            cout << "Time:      " << temp->timestamp        << endl;
            cout << "-----------------------------------"    << endl;
            temp = temp->next;
        }
    }
};

// ================= 2. EMERGENCY REQUESTS (QUEUE) =================
struct Emergency {
    int    id;
    string caller;
    string location;
    string details;
};

class BatSignalQueue {
private:
    queue<Emergency> q;
    int counter = 100;

public:
    void enqueueRequest(string caller, string loc, string details) {
        Emergency e = {++counter, caller, loc, details};
        q.push(e);
        cout << YELLOW << "[BAT-SIGNAL]: Alert received from " << caller << " at " << loc << ". Added to queue." << RESET << endl;
    }

    void respondToNext() {
        if (q.empty()) {
            cout << GREEN << "[ALFRED]: All clear, Master Wayne. No pending distress signals." << RESET << endl;
            return;
        }
        Emergency e = q.front();
        q.pop();
        cout << CYAN << "Responding to ID #" << e.id << "..." << RESET << endl;
        cout << "Deploying to " << e.location << ". Objective: " << e.details << endl;
    }

    void displayPending() {
        printHeader("PENDING BAT-SIGNAL ALERTS");
        if (q.empty()) { cout << "Queue empty." << endl; return; }
        queue<Emergency> temp = q;
        while (!temp.empty()) {
            Emergency e = temp.front();
            cout << "ID: " << e.id << " | Loc: " << e.location << " | Details: " << e.details << endl;
            temp.pop();
        }
    }
};

// ================= 3. GOTHAM NAVIGATION (WEIGHTED GRAPH) =================
class GothamNavigation {
private:
    unordered_map<string, vector<pair<string, int>>> adj;

public:
    void addRoad(string u, string v, int weight) {
        adj[u].push_back({v, weight});
        adj[v].push_back({u, weight});
    }

    void findOptimalPath(string start, string end) {
        printHeader("CALCULATING OPTIMAL ROUTE");

        unordered_map<string, int> dist;
        unordered_map<string, string> parent;        
        for (auto const& entry : adj)
            dist[entry.first] = INT_MAX;

        priority_queue<pair<int,string>,
                       vector<pair<int,string>>,
                       greater<pair<int,string>>> pq;

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            string u = pq.top().second;
            int    d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;
            if (u == end)    break;

            for (auto& edge : adj[u]) {
                string v      = edge.first;
                int    weight = edge.second;
                if (dist[u] + weight < dist[v]) {
                    dist[v]   = dist[u] + weight;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[end] == INT_MAX) {
            cout << RED << "[COMPUTER]: No viable route found. Roadblocks suspected." << RESET << endl;
            return;
        }

        // Reconstruct path
        vector<string> path;
        for (string at = end; at != ""; at = parent[at]) {
            path.push_back(at);
            if (at == start) break;
        }
        reverse(path.begin(), path.end());

        cout << GREEN << "[BAT-COMPUTER]: Route calculated." << RESET << endl;
        cout << "Path: ";
        for (size_t i = 0; i < path.size(); i++)
            cout << path[i] << (i < path.size() - 1 ? " -> " : "");
        cout << "\nTotal Distance/Risk Factor: " << dist[end] << endl;
    }

    void initMap() {
        addRoad("Wayne Tower",   "GCPD",          5);
        addRoad("Wayne Tower",   "Crime Alley",   8);
        addRoad("GCPD",          "Arkham Asylum", 12);
        addRoad("Crime Alley",   "The Narrows",   6);
        addRoad("The Narrows",   "Arkham Asylum", 4);
        addRoad("GCPD",          "Iceberg Lounge",7);
    }
};

// ================= 4. CRIME HOTSPOTS (BST) =================
struct CrimeNode {
    int dangerIndex;
    string location;
    CrimeNode *left, *right;
    CrimeNode(int d, string l) : dangerIndex(d), location(l), left(nullptr), right(nullptr) {}
};

class CrimeAnalysis {
private:
    CrimeNode* root;

    CrimeNode* insert(CrimeNode* node, int danger, string loc) {
        if (!node) return new CrimeNode(danger, loc);
        if (danger < node->dangerIndex)
            node->left  = insert(node->left,  danger, loc);
        else
            node->right = insert(node->right, danger, loc);
        return node;
    }

    void inorder(CrimeNode* node) {
        if (!node) return;
        inorder(node->left);
        cout << "Danger Lvl [" << node->dangerIndex << "]: " << node->location << endl;
        inorder(node->right);
    }

public:
    CrimeAnalysis() : root(nullptr) {}

    void reportCrime(int danger, string loc) {
        root = insert(root, danger, loc);
        cout << RED << "[COMPUTER]: New data point added to Hotspot Matrix. Threat Level: " << danger << RESET << endl;
    }

    void displayHotspots() {
        printHeader("CRIME HOTSPOT ANALYSIS (SORTED BY DANGER)");
        inorder(root);
    }
};

// ================= 5. BATARANG INVENTORY (STACK) =================
class BatarangInventory {
private:
    stack<string> inv;

public:
    void addGadget(string gadget) {
        inv.push(gadget);
        cout << GREEN << "[ALFRED]: '" << gadget << "' loaded into utility belt." << RESET << endl;
    }

    void useGadget() {
        if (inv.empty()) {
            cout << RED << "[ALFRED]: Utility belt is empty, Master Wayne!" << RESET << endl;
            return;
        }
        cout << CYAN << "[BATMAN]: Deploying " << inv.top() << " !" << RESET << endl;
        inv.pop();
        cout << GREEN << "[ALFRED]: Item removed from belt." << RESET << endl;
    }

    void viewBelt() {
        printHeader("UTILITY BELT  (TOP = MOST RECENT)");
        if (inv.empty()) { cout << "Belt is empty." << endl; return; }
        stack<string> temp = inv;
        int slot = 1;
        while (!temp.empty()) {
            cout << "  [" << slot++ << "] " << temp.top() << endl;
            temp.pop();
        }
    }
};

// ================= 6. THREAT PRIORITIZER (MAX-HEAP) =================
struct Threat {
    int    threatLevel;
    string villainName;

    // Makes std::make_heap / push_heap behave as a MAX-heap
    bool operator<(const Threat& o) const {
        return threatLevel < o.threatLevel;
    }
};

class ThreatPrioritizer {
private:
    vector<Threat> heap;   // underlying storage; heap property maintained manually

public:
    void addThreat(string villain, int level) {
        heap.push_back({level, villain});
        push_heap(heap.begin(), heap.end());   // re-heapify after insert
        cout << YELLOW << "[COMPUTER]: Threat logged — " << villain
             << "  |  Level: " << level << RESET << endl;
    }

    void neutralizeTopThreat() {
        if (heap.empty()) {
            cout << GREEN << "[ALFRED]: No active threats in the matrix, sir." << RESET << endl;
            return;
        }
        // pop_heap moves the max element to the back; we then remove it
        pop_heap(heap.begin(), heap.end());
        Threat t = heap.back();
        heap.pop_back();
        cout << CYAN << "[BATMAN]: Neutralizing highest threat: " << t.villainName
             << "  (Level " << t.threatLevel << ")" << RESET << endl;
    }

    void displayThreats() {
        printHeader("ACTIVE THREAT LEVELS (MAX-HEAP)");
        if (heap.empty()) { cout << "No threats detected." << endl; return; }

        // Copy and sort descending for clean display (does NOT disturb the real heap)
        vector<Threat> temp = heap;
        sort(temp.begin(), temp.end(), [](const Threat& a, const Threat& b){
            return a.threatLevel > b.threatLevel;
        });
        for (auto& t : temp) {
            cout << RED << "  >> " << t.villainName
                 << "  |  Threat Level: " << t.threatLevel << RESET << endl;
        }
    }
};

// ================= BAT-LOGO =================
void printBatLogo() {
    cout << YELLOW << R"(
       _==/          i     i          \==_
     /XX/            |\___/|            \XX\
   /XXXX\            |XXXXX|            /XXXX\
  |XXXXXX\_         _XXXXXXX_         _/XXXXXX|
   XXXXXXXXXXXxxxxxxxXXXXXXXXXXXxxxxxxxXXXXXXXXXXX
  |XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|
   XXXXXX/^^^^"\XXXXXXXXXXXXXXXXXXXXX/^^^^^\XXXXXX
   |XXX|       \XXX/^^\XXXXX/^^\XXX/       |XXX|
     \XX\       \X/    \XXX/    \X/       /XX/
        "\       "      \X/      "      /"
    )" << RESET << endl;
    cout << setw(50) << "WAYNE ENTERPRISES TECH DIV."  << endl;
    cout << setw(48) << "BCPVTS v2.4 ONLINE"            << endl;
}

// ================= MAIN =================
int main() {
    VillainTracker    tracker;
    BatSignalQueue    emergencyQueue;
    GothamNavigation  gps;
    CrimeAnalysis     analytics;
    BatarangInventory belt;          // NEW — Stack
    ThreatPrioritizer threatHeap;    // NEW — Heap

    // --- PRE-LOAD DUMMY DATA ---
    gps.initMap();

    tracker.addSighting("The Joker", "Ace Chemicals",  "Hijacked chemical shipment", "23:45");
    tracker.addSighting("Bane",      "Gotham Stadium", "Planted explosives",          "00:15");

    emergencyQueue.enqueueRequest("Comm. Gordon", "GCPD Rooftop",  "Bat-Signal lit. Immediate assistance required.");
    emergencyQueue.enqueueRequest("Oracle",       "Clock Tower",   "Hacking attempt detected on WayneNet.");

    analytics.reportCrime(85, "The Narrows - Gang War");
    analytics.reportCrime(95, "Arkham Asylum - Breakout in progress");
    analytics.reportCrime(40, "Robinson Park - Mugging");

    // Stack pre-load
    belt.addGadget("Batarang");
    belt.addGadget("Grappling Hook");
    belt.addGadget("Smoke Pellet");

    // Heap pre-load
    threatHeap.addThreat("Scarecrow", 72);
    threatHeap.addThreat("Bane",      88);
    threatHeap.addThreat("The Joker", 95);

    // --- BOOT SCREEN ---
    printBatLogo();
    cout << "\n[SYSTEM]: BIOMETRIC SCAN... COMPLETE." << endl;
    cout << "[SYSTEM]: WELCOME, MASTER WAYNE."        << endl;

    int choice;
    do {
        cout << "\n" << CYAN << "=== MAIN CONTROL TERMINAL ===" << RESET << endl;
        cout << "1. Villain Tracker      (Linked List)"  << endl;
        cout << "2. Bat-Signal Queue     (FIFO Queue)"   << endl;
        cout << "3. Gotham Navigation    (Dijkstra)"     << endl;
        cout << "4. Crime Analysis       (BST)"          << endl;
        cout << "5. Utility Belt         (Stack)"        << endl;
        cout << "6. Threat Prioritizer   (Max-Heap)"     << endl;
        cout << "0. Log Out / Shutdown"                  << endl;
        cout << "Select Protocol: ";
        cin  >> choice;

        switch (choice) {
            // ---------- LINKED LIST ----------
            case 1: {
                int vChoice;
                cout << "\n-- VILLAIN TRACKING --\n1. Add Sighting\n2. View Log\n3. Mark Resolved\nChoice: ";
                cin >> vChoice;
                if (vChoice == 1) {
                    string n, l, c, t;
                    cout << "Villain Name: "; cin.ignore(); getline(cin, n);
                    cout << "Location:     "; getline(cin, l);
                    cout << "Crime:        "; getline(cin, c);
                    cout << "Time:         "; getline(cin, t);
                    tracker.addSighting(n, l, c, t);
                } else if (vChoice == 2) {
                    tracker.displayTimeline();
                } else if (vChoice == 3) {
                    string n;
                    cout << "Villain Name to remove: "; cin.ignore(); getline(cin, n);
                    tracker.resolveSighting(n);
                }
                break;
            }
            // ---------- QUEUE ----------
            case 2: {
                int qChoice;
                cout << "\n-- EMERGENCY QUEUE --\n1. View Pending\n2. Respond to Alert\nChoice: ";
                cin >> qChoice;
                if (qChoice == 1) emergencyQueue.displayPending();
                if (qChoice == 2) emergencyQueue.respondToNext();
                break;
            }
            // ---------- GRAPH ----------
            case 3: {
                string start, dest;
                cout << "\n-- NAVIGATION SYSTEM --" << endl;
                cout << "Known locations: Wayne Tower, GCPD, Crime Alley, The Narrows, Arkham Asylum, Iceberg Lounge" << endl;
                cout << "Start Point: "; cin.ignore(); getline(cin, start);
                cout << "Destination: "; getline(cin, dest);
                gps.findOptimalPath(start, dest);
                break;
            }
            // ---------- BST ----------
            case 4: {
                int cChoice;
                cout << "\n-- CRIME HEATMAP --\n1. Add Crime Data\n2. View Risk Levels\nChoice: ";
                cin >> cChoice;
                if (cChoice == 1) {
                    int d; string l;
                    cout << "Danger Index (1-100): "; cin >> d;
                    cout << "Location: "; cin.ignore(); getline(cin, l);
                    analytics.reportCrime(d, l);
                }
                if (cChoice == 2) analytics.displayHotspots();
                break;
            }
            // ---------- STACK ----------
            case 5: {
                int sChoice;
                cout << "\n-- UTILITY BELT (STACK) --\n1. Add Gadget\n2. Use Top Gadget\n3. View Belt\nChoice: ";
                cin >> sChoice;
                if (sChoice == 1) {
                    string g;
                    cout << "Gadget name: "; cin.ignore(); getline(cin, g);
                    belt.addGadget(g);
                } else if (sChoice == 2) {
                    belt.useGadget();
                } else if (sChoice == 3) {
                    belt.viewBelt();
                }
                break;
            }
            // ---------- HEAP ----------
            case 6: {
                int hChoice;
                cout << "\n-- THREAT PRIORITIZER (MAX-HEAP) --\n1. Add Threat\n2. Neutralize Top Threat\n3. View All Threats\nChoice: ";
                cin >> hChoice;
                if (hChoice == 1) {
                    string v; int lvl;
                    cout << "Villain Name: "; cin.ignore(); getline(cin, v);
                    cout << "Threat Level (1-100): "; cin >> lvl;
                    threatHeap.addThreat(v, lvl);
                } else if (hChoice == 2) {
                    threatHeap.neutralizeTopThreat();
                } else if (hChoice == 3) {
                    threatHeap.displayThreats();
                }
                break;
            }
            case 0:
                cout << GREEN << "[ALFRED]: Goodnight, Master Wayne. Systems powering down." << RESET << endl;
                break;
            default:
                cout << RED << "Invalid Command." << RESET << endl;
        }

    } while (choice != 0);

    return 0;
}
