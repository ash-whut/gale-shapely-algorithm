#include <iostream>
#include <vector>
#include <fstream>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split

using namespace std;

class Person {
    private:
        string name;
        Person* partner;
        bool matched;

    public:
        Person(){
            this->name = "";
            this->matched = false;
            this->partner = NULL;
        }

        Person(string name) {
            this->name = name;
            this->matched = false;
            this->partner = NULL;
        }

        string getName() {
            return this->name;
        }

        bool isMatched() {
            return this->matched;
        }

        Person* getPartner() {
            return this->partner;
        }

        void setPartner(Person* partner) {
            Person* temp_partner = this->partner;
            temp_partner->partner = NULL;
            temp_partner->matched = false;

            this->partner = partner;
        }
};

class GS_Algorithm {
    private:
        vector<Person*> boys;
        vector<Person*> girls;
        vector<vector<Person>> boys_preferences;
        vector<vector<Person>> girls_preferences;
        vector<vector<Person*>> final_pairs;
        int number_of_people_each_set;
    
    public:
        GS_Algorithm() {
            cout << "Enter number of people in each set (n): ";
            cin >> number_of_people_each_set;
            cout << endl;

            ifstream myfile ("boys.txt");
            string boy_name;

            if (myfile.is_open())
            {
                while (getline(myfile, boy_name))
                    {
                        Person* person_pointer = new Person(boy_name);
                        boys.push_back(person_pointer);
                    }

                myfile.close();
            }     

            ifstream myfile2 ("girls.txt");
            string girl_name;

            if (myfile2.is_open())
            {
                while (getline(myfile2, girl_name))
                    {
                        Person* person_pointer = new Person(girl_name);
                        girls.push_back(person_pointer);
                    }

                myfile2.close();
            }

            ifstream myfile3 ("boysP.txt");
            string boys_;

            if (myfile3.is_open())
            {
                int index = 0;

                while (getline(myfile3, boys_))
                    {
                        vector<string> preferences;
                        vector<Person> preferences_;

                        boost::split(preferences, boys_, boost::is_any_of(" "), boost::token_compress_on);  

                        for (int i = 0; i < number_of_people_each_set; i++)
                        {
                            Person person = Person(preferences.at(i));
                            preferences_.push_back(person);
                        }

                        boys_preferences.push_back(preferences_);

                        index++;
                    }

                myfile3.close();               
            }

            ifstream myfile4 ("girlsP.txt");
            string girls_;

            if (myfile4.is_open())
            {
                int index = 0;

                while (getline(myfile4, girls_))
                    {
                        vector<string> preferences;
                        vector<Person> preferences_;

                        boost::split(preferences, girls_, boost::is_any_of(" "), boost::token_compress_on);  

                        for (int i = 0; i < number_of_people_each_set; i++)
                        {
                            Person person = Person(preferences.at(i));
                            preferences_.push_back(person);
                        }

                        girls_preferences.push_back(preferences_);

                        index++;                        

                        girls_preferences.push_back(preferences_);
                    }

                myfile3.close();               
            }
        }

        // Helper function to check if there are any free boys every loop
        // and return them as a vector.
        vector<Person*> freeBoys()
        {
            vector<Person*> returned_vector;

            for (int i = 0; i < boys.size(); i++)
            {
                if (boys.at(i)->isMatched() == false)
                {
                    returned_vector.push_back(boys.at(i));
                }
            }

            return returned_vector;
        }

        void CreatePairs()
        {
            vector<Person*> freeBoys = this->freeBoys();

            // Loop runs as long as there aren't free boys
            // which implies that there are no more pairs to be made.
            while (!freeBoys.empty())
            {
                for (int i = 0; i < freeBoys.size(); i++)
                {
                    Person* freeBoy = freeBoys.at(i);

                }
            }

        }

        void printPairs() {
            cout << "Final Pairs:\n" << endl;
            for (int i = 0; i < final_pairs.size(); i++) {
                cout << final_pairs[i][0]->getName() << " - " << final_pairs[i][1]->getName() << endl;
            }
        }

        void printBoysPreferences() {
            cout << "Boys Preferences:\n" << endl;
            for (int i = 0; i < boys_preferences.size(); i++) {
                cout << boys.at(i)->getName() << ": ";
                for (int j = 0; j < boys_preferences[i].size(); j++) {
                    cout << boys_preferences[i][j].getName() << " ";
                }
                cout << endl;
            }
        }

        void printGirlsPreferences() {
            cout << "Girls Preferences:\n" << endl;
            for (int i = 0; i < girls_preferences.size(); i++) {
                cout << girls.at(i)->getName() << ": ";
                for (int j = 0; j < girls_preferences[i].size(); j++) {
                    cout << girls_preferences[i][j].getName() << " ";
                }
                cout << endl;
            }
        }


};

int main() {
    GS_Algorithm gs = GS_Algorithm();
    gs.printBoysPreferences();
    cout << "\n" << endl;
    gs.printGirlsPreferences();
    cout << "\n" << endl;
    gs.createPairs();
    return 0;
}