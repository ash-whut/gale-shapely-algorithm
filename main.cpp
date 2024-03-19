#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
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

            if (temp_partner != NULL)
            {
                temp_partner->partner = NULL;
                temp_partner->matched = false;
            }

            this->partner = partner;
            this->matched = true;
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

        int indexOfPerson(vector<Person*> people, Person* person)
        {
            for (int i = 0; i < people.size(); i++)
            {
                if (people.at(i)->getName() == person->getName())
                {
                    return i;
                }
            }

            return -1;
        }

        int indexOfPerson(vector<Person> people, Person* person)
        {
            for (int i = 0; i < people.size(); i++)
            {
                if (people.at(i).getName() == person->getName())
                {
                    return i;
                }
            }

            return -1;
        }

        int indexOfPerson(vector<Person> people, Person person)
        {
            for (int i = 0; i < people.size(); i++)
            {
                if (people.at(i).getName() == person.getName())
                {
                    return i;
                }
            }

            return -1;
        }

        void createPairs()
        {
            vector<Person*> freeBoys = this->freeBoys();

            // Loop runs as long as there aren't free boys
            // which implies that there are no more pairs to be made.
            while (!freeBoys.empty())
            {
                for (int i = 0; i < freeBoys.size(); i++)
                {
                    Person* freeBoy = freeBoys.at(i);
                    int index = indexOfPerson(boys, freeBoy);
                    vector<Person> preferences = boys_preferences.at(index);

                    for (int j = 0; j < preferences.size(); j++)
                    {
                        Person girl = preferences.at(j);
                        int girl_index = indexOfPerson(girls, &girl);
                        vector<Person> girl_preferences = girls_preferences.at(girl_index);
                        Person* girl_from_girls_list = girls.at(girl_index);

                        if (girl_from_girls_list->isMatched())
                        {
                            Person* girl_current_partner = girl_from_girls_list->getPartner();
                            int current_partner_index = indexOfPerson(girl_preferences, *girl_current_partner);
                            int new_partner_index = indexOfPerson(girl_preferences, *freeBoy);

                            if (new_partner_index < current_partner_index)
                            {
                                girl_from_girls_list->setPartner(freeBoy);
                                freeBoy->setPartner(girl_from_girls_list);
                                break;
                            }

                        }

                        else
                        {
                            freeBoy->setPartner(girls.at(girl_index));
                            girls.at(girl_index)->setPartner(freeBoy);
                            break;
                        }

                    }
                }

                freeBoys = this->freeBoys();

            }

        }

        void parsePairs()
        {
            for (int i = 0; i < boys.size(); i++)
            {
                vector<Person*> pair;
                pair.push_back(boys.at(i));
                pair.push_back(boys.at(i)->getPartner());

                final_pairs.push_back(pair);
            }
        }

        void printPairs() {
            cout << "Final Pairs:\n" << endl;
            for (int i = 0; i < final_pairs.size(); i++) {
                cout << "(" << final_pairs[i][0]->getName() << ", " << final_pairs[i][1]->getName() << ")" << endl;
            }
        }

        void printBoysPreferences() {
            cout << "Boys Preferences:\n" << endl;
            for (int i = 0; i < boys_preferences.size(); i++) {
                cout << setw(10) << left << boys.at(i)->getName() << ": ";
                for (int j = 0; j < boys_preferences[i].size(); j++) {
                    cout << setw(10) << left << boys_preferences[i][j].getName();
                }
                cout << endl;
            }
        }

        void printGirlsPreferences() {
            cout << "Girls Preferences:\n" << endl;
            for (int i = 0; i < girls_preferences.size(); i++) {
                cout << setw(10) << left << girls.at(i)->getName() << ": ";
                for (int j = 0; j < girls_preferences[i].size(); j++) {
                    cout << setw(10) << left << girls_preferences[i][j].getName();
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
    gs.parsePairs();
    gs.printPairs();
    return 0;
}