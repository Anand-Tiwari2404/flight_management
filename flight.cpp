#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

class Flight
{
private:
    string flightName;
    int flightID;
    int capacity;
    string arrivalTime;
    string departureTime;
    string flightClass;
    int delayTime;
    string status;

public:
    // == overloading
    bool operator==(const Flight &other) const
    {
        return flightID == other.flightID && flightName == other.flightName && arrivalTime == other.arrivalTime;
    }

    Flight(string name, int id, int cap, string arrival, string departure, string fClass, int delay = 0, string stat = "On time")
        : flightName(name), flightID(id), capacity(cap), arrivalTime(arrival), departureTime(departure), flightClass(fClass), delayTime(delay), status(stat) {}

    string getFlightName() const { return flightName; }
    int getFlightID() const { return flightID; }
    int getCapacity() const { return capacity; }
    string getArrivalTime() const { return arrivalTime; }
    string getDepartureTime() const { return departureTime; }
    string getFlightClass() const { return flightClass; }
    int getDelayTime() const { return delayTime; }
    string getStatus() const { return status; }
    void setStatus(const string &newStatus) { status = newStatus; }
    void setDelayTime(int delay) { delayTime = delay; }

    // Display function
    void display() const
    {
        cout << left << setw(20) << flightName
             << setw(10) << flightID
             << setw(10) << capacity
             << setw(15) << arrivalTime
             << setw(15) << departureTime
             << setw(15) << flightClass
             << setw(10) << status
             << setw(10) << delayTime
             << endl;
    }

    bool operator<(const Flight &other) const
    {
        return flightID < other.flightID || (flightID == other.flightID && arrivalTime < other.arrivalTime);
    }
};

class FlightManager
{
private:
    vector<Flight> flights;

    void logAction(const string &action) const
    {
        ofstream logfile("logfile.txt", ios::app);
        if (logfile.is_open())
        {
            logfile << action << endl;
            logfile.close();
        }
    }

public:
    void displayFlights() const
    {
        if (flights.empty())
        {
            cout << "No flight records to display.\n";
            logAction("Displayed flight list: No records to display");
            return;
        }

        cout << left << setw(20) << "Flight Name"
             << setw(10) << "ID"
             << setw(10) << "Capacity"
             << setw(15) << "Arrival"
             << setw(15) << "Departure"
             << setw(15) << "Class"
             << setw(10) << "Status"
             << setw(10) << "Delay (mins)"
             << endl;
        cout << string(112, '-') << endl;

        for (const auto &flight : flights)
        {
            flight.display();
        }
        cout << endl;
        logAction("Displayed flight list");
    }

    void insertFlight(const Flight &newFlight)
    {
        flights.push_back(newFlight);
        sort(flights.begin(), flights.end());
        cout << "Flight inserted successfully.\n";
        logAction("Flight inserted: " + newFlight.getFlightName() + " with ID " + to_string(newFlight.getFlightID()));
    }

    void updateFlight(int flightID, const string &arrivalTime, const string &newStatus, int delayTime)
    {
        bool updated = false;
        for (auto &flight : flights)
        {
            if (flight.getFlightID() == flightID && flight.getArrivalTime() == arrivalTime)
            {
                flight.setStatus(newStatus);
                flight.setDelayTime(delayTime);
                updated = true;
                logAction("Flight updated: ID " + to_string(flightID) + ", Arrival Time " + arrivalTime + " to " + newStatus + " with delay " + to_string(delayTime) + " mins");
            }
        }
        cout << (updated ? "Flight(s) updated successfully.\n" : "Flight not found for update.\n");
    }

    bool deleteFlight(int flightID, const string &arrivalTime)
    {
        auto it = remove_if(flights.begin(), flights.end(), [=](const Flight &f)
                            { return f.getFlightID() == flightID && f.getArrivalTime() == arrivalTime; });
        if (it != flights.end())
        {
            flights.erase(it, flights.end());
            cout << "Flight deleted successfully.\n";
            logAction("Flight deleted: ID " + to_string(flightID) + ", Arrival Time " + arrivalTime);
            return true;
        }
        else
        {
            cout << "Flight not found.\n";
            logAction("Flight deletion failed: ID " + to_string(flightID) + ", Arrival Time " + arrivalTime + " not found");
            return false;
        }
    }

    void listUnique()
    {
        sort(flights.begin(), flights.end());
        auto last = unique(flights.begin(), flights.end(), [](const Flight &a, const Flight &b)
                           { return a.getFlightID() == b.getFlightID() && a.getArrivalTime() == b.getArrivalTime(); });
        flights.erase(last, flights.end());
        cout << "List after removing duplicates:\n";
        displayFlights();
    }

    void listUnion(const FlightManager &other)
    {
        vector<Flight> unionList = flights;
        unionList.insert(unionList.end(), other.flights.begin(), other.flights.end());
        sort(unionList.begin(), unionList.end());
        unionList.erase(unique(unionList.begin(), unionList.end(), [](const Flight &a, const Flight &b)
                               { return a.getFlightID() == b.getFlightID() && a.getArrivalTime() == b.getArrivalTime(); }),
                        unionList.end());

        cout << "Union of lists:\n";
        for (const auto &flight : unionList)
            flight.display();
    }

    void listIntersection(const FlightManager &other)
    {
        vector<Flight> intersectionList;
        for (const auto &flight1 : flights)
        {
            for (const auto &flight2 : other.flights)
            {
                if (flight1.getFlightID() == flight2.getFlightID() && flight1.getArrivalTime() == flight2.getArrivalTime())
                {
                    intersectionList.push_back(flight1);
                    break;
                }
            }
        }

        sort(intersectionList.begin(), intersectionList.end());
        intersectionList.erase(unique(intersectionList.begin(), intersectionList.end()), intersectionList.end());

        cout << "Intersection of lists:\n";
        for (const auto &flight : intersectionList)
            flight.display();
    }

    void listDifference(const FlightManager &other)
    {
        vector<Flight> differenceList;

        for (const auto &flight : flights)
        {

            bool found = false;
            for (const auto &otherFlight : other.flights)
            {
                if (flight == otherFlight)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                differenceList.push_back(flight);
            }
        }

        cout << "Difference of lists:\n";
        for (const auto &flight : differenceList)
            flight.display();
    }

    void listSymmetricDifference(const FlightManager &other)
    {
        vector<Flight> symmetricDifferenceList;

        for (const auto &flight : flights)
        {
            bool found = false;
            for (const auto &otherFlight : other.flights)
            {
                if (flight == otherFlight)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                symmetricDifferenceList.push_back(flight);
            }
        }

        for (const auto &otherFlight : other.flights)
        {
            bool found = false;
            for (const auto &flight : flights)
            {
                if (otherFlight == flight)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                symmetricDifferenceList.push_back(otherFlight);
            }
        }

        cout << "Symmetric Difference of lists:\n";
        for (const auto &flight : symmetricDifferenceList)
            flight.display();
    }

    int numOfFlights() const
    {
        return flights.size();
    }

    bool isEmpty() const
    {
        return flights.empty();
    }

    void getLongestDelay() const
    {
        if (flights.empty())
        {
            cout << "No flights available to find the longest delay.\n";
            return;
        }
        auto maxDelayFlight = max_element(flights.begin(), flights.end(), [](const Flight &a, const Flight &b)
                                          { return a.getDelayTime() < b.getDelayTime(); });
        cout << "Flight with the longest delay:\n";
        maxDelayFlight->display();
    }
};

int main()
{

    FlightManager flightManager1;
    FlightManager flightManager2;

    cout << "-------------------------- FLIGHT MANAGEMENT SYSTEM --------------------------" << endl;
    cout << endl;
    flightManager1.insertFlight({"Air India", 101, 150, "0800", "1000", "VIP", 10});
    cout << endl;
    flightManager1.insertFlight({"Emirates", 102, 200, "0900", "1100", "Public", 20});
    cout << endl;
    flightManager1.insertFlight({"Air India", 101, 150, "0800", "1000", "VIP", 10}); // Duplicate flight for testing
    cout << endl;
    flightManager1.displayFlights();
    cout << endl;
    flightManager1.getLongestDelay();
    cout << endl;
    cout << "Number of flights in flightManager1: " << flightManager1.numOfFlights() << endl;
    cout << endl;
    flightManager1.updateFlight(101, "0800", "Delayed", 30);
    cout << endl;
    flightManager1.deleteFlight(102, "0900");
    cout << endl;
    flightManager1.displayFlights();
    cout << endl;
    flightManager2.insertFlight({"British Airways", 103, 180, "1000", "1200", "Business", 5});
    flightManager2.insertFlight({"Emirates", 102, 200, "0900", "1100", "Public", 25}); // Flight already exists in flightManager1
    cout << endl;
    flightManager2.displayFlights();
    cout << endl;
    flightManager1.listUnique();
    cout << endl;
    flightManager1.listUnion(flightManager2);
    cout << endl;
    flightManager1.listSymmetricDifference(flightManager2);
    cout << endl;
    cout << (flightManager1.isEmpty() ? "Flight Manager Unit 1 is empty" : "Flight Manager Unit 1 is not empty") << endl;
    cout << endl;
    return 0;
}