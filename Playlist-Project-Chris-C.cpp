// Chris Coleman
// 1/29/2025
// Playlist Maker
// Extra: files

/*
Need some songs to test out?

Never gonna give you up:
https://www.youtube.com/watch?v=dQw4w9WgXcQ

Heartless:
https://www.youtube.com/watch?v=Co0tTeuUVhU



*/

#include <iostream>
#include <algorithm>
#include <fstream>
#include <random>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <cstdlib>

using namespace std;

vector<string> songs;
vector<string> links;
vector<string> playlist;
vector<string> playlistLinks;
vector<string> queue;

void listen();
void add();
void rm();
void db();
void loadSongs();
void split(string input, string &song, string &artist); // splits the song and artist
void list(vector<string> sons);
void copy(const vector<string> &from, vector<string> &to);
void openLink(const std::string &url);
void cs(); // Clears the screen (contributed by deepseek)

int main()
{
    loadSongs();
    while (true)
    {
        cout << "---------------- P L A Y L I S T ---- M A K E R ----------------" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Listen to playlist" << endl;
        cout << "2. Add songs to playlist" << endl;
        cout << "3. Remove songs from playlist" << endl;
        cout << "4. Add songs to database" << endl;
        cout << "5. Reload Songs" << endl;
        cout << "6. Exit program" << endl;
        int choice;
        cin >> choice;
        cin.ignore();
        switch (choice)
        {
        case 1:
            cs();
            listen();
            cout << "\n\nThank you for using the playlist maker!" << endl;
            return 0;
        case 2:
            cs();
            add();
            cs();
            break;
        case 3:
            cs();
            rm();
            cs();
            break;
        case 4:
            cs();
            db();
            cs();
            break;
        case 5:
            cs();
            loadSongs();
            break;
        case 6:
            cout << "\n\nThank you for using the playlist maker!" << endl;
            return 0;
        default:
            cs();
            cout << "Please enter a number 1 - 6" << endl;
            return 1;
        }
    }
}

void loadSongs()
{
    cout << "Loading songs..." << endl;
    string text;
    try
    { // Will try to run the code below
        ifstream songsFile("songs.txt");
        ifstream linksFile("links.txt");

        if (!songsFile.is_open() || !linksFile.is_open())
        {
            throw runtime_error("Error opening file - make sure you have a songs.txt and links.txt file"); // Throws an error that it can't find the file
        }

        // Empty vectors so we can put in new data
        songs.clear();
        links.clear();

        while (getline(songsFile, text))
        {
            songs.push_back(text); // put in the song
        }
        while (getline(linksFile, text))
        {
            links.push_back(text); // put in the link
        }
        songsFile.close();
        linksFile.close();
        list(songs);
        list(links);
        cout << "Songs loaded!" << endl;
    }
    catch (exception &e)
    {
        cout << "Error loading songs: " << e.what() << endl; // reports the error
    }
}

void db()
{
    cout << "Please enter the name of the song you would like to add" << endl;
    cout << "Please enter the name using this format: Not Like Us - Kendrick Lamar" << endl;
    string name;
    getline(cin, name);

    cout << "Please enter the FULL youtube link" << endl;
    string link;

    // takes out leftover newline characters from input buffer
    if (cin.peek() == '\n')
    {                 // if there is a newline character
        cin.ignore(); // ignores the next new line character
    }
    getline(cin, link);
    // opening file using ofstream
    ofstream songsFile("songs.txt", ios::app);
    ofstream linksFile("links.txt", ios::app);

    songsFile << name << endl;
    linksFile << link << endl;
}

void listen()
{
    copy(playlist, queue);
    list(queue);
    random_device rd;
    mt19937 g(rd());
    shuffle(queue.begin(), queue.end(), g); // shuffles the music using something - c++ 17 or more
    list(queue);
    for (int i = 0; i < queue.size(); i++)
    {
        cout << "Now Playing: " << endl;
        string currentSong;
        string currentArtist;
        split(queue.at(i), currentSong, currentArtist);
        cout << currentSong << " by " << currentArtist << endl;
        openLink(playlistLinks.at(i));
        this_thread::sleep_for(chrono::seconds(10));
    }
    cout << "Done Playing Playlist" << endl;
}
void add()
{
    while (true)
    {
        cs();
        cout << "This is every song on our platform:" << endl;
        cout << "\n-----------------------------------------\n"
             << endl;
        list(songs);
        cout << "\n-----------------------------------------\n"
             << endl;
        cout << "Please enter the number of the song you want to add to YOUR playlist (enter -1 to exit): " << endl;
        int choice;
        cin >> choice;
        cin.ignore();
        if (choice == -1)
        {
            break;
        }
        string currentSong;
        string currentArtist;
        split(songs.at(choice), currentSong, currentArtist);
        playlist.push_back(songs.at(choice));
        playlistLinks.push_back(links.at(choice));
        cout << "\n-----------------------------------------\n"
             << endl;
        cout << currentSong << " by " << currentArtist << " has been added!" << endl;
        cout << "\n-----------------------------------------\n"
             << endl;
    }
}

void rm()
{
    while (true)
    {
        cs();
        cout << "This is every song in YOUR playlist:" << endl;
        cout << "\n-----------------------------------------\n"
             << endl;
        list(playlist);
        cout << "\n-----------------------------------------\n"
             << endl;
        cout << "Please enter the number of the song you want to remove from YOUR playlist (enter -1 to exit): "
             << endl;
        int choice;
        cin >> choice;
        cin.ignore();
        if (choice == -1)
        {
            break;
        }
        playlist.erase(playlist.begin() + choice);
        playlistLinks.erase(playlistLinks.begin() + choice);
        cout << "Sucesssfully Removed!" << endl;
    }
}

void list(vector<string> sons)
{
    for (int i = 0; i < sons.size(); i++)
    {
        cout << i << ". " << sons.at(i) << endl;
    }
}

void openLink(const std::string &url)
{ // chatgpt made this part :P
#ifdef _WIN32
    // Windows
    std::string command = "start " + url;
    system(command.c_str());
#elif __APPLE__
    // macOS
    std::string command = "open " + url;
    system(command.c_str());
#elif __linux__
    // Linux
    std::string command = "xdg-open " + url;
    system(command.c_str());
#else
    std::cerr << "Unsupported OS" << std::endl;
#endif
}

void copy(const vector<string> &from, vector<string> &to)
{
    to = from;
}

void split(string input, string &song, string &artist)
{
    int pos = input.find(" - ");
    song = input.substr(0, pos); // Goes from begginning to the position of the - character
    // Goes from - to the end of the string
    artist = input.substr(pos + 3); // - and the +3 because - is 3 characters long
}

void cs()
{
    {
#ifdef _WIN32
        // Windows
        system("cls");
#else
        // Unix-like systems (Linux, macOS)
        system("clear");
#endif
    }
}
