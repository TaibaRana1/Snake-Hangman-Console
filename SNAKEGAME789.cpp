#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
using namespace sf;
using namespace std;


const int maxWidth = 800;
const int maxHeight = 600;
const int MAX_TAIL_SIZE = 100;
const int MAX_FOOD = 7;
const int MAX_Hurdle = 5;
int score = 0;
int headX = 200, headY = 200;


//player HAS achievements. Achievements without a player doesnt make sense
class Achievements;
class player {
public:
    std::string name;
    int player_snake_score;
    int player_wordle_score;
    int player_hangman_score;

    player()
        : name(""), player_snake_score(0), player_wordle_score(0), player_hangman_score(0) {}

    player(const std::string& _name, int ss, int ws, int hs)
        : name(_name), player_snake_score(ss), player_wordle_score(ws), player_hangman_score(hs) {}

    void setPlayersDetail() {
        
        sf::RenderWindow window(sf::VideoMode(800, 600), "Player Input");

        
        sf::Font font;
        if (!font.loadFromFile("Roboto-Regular.ttf")) 
        {
            cout << "Failed to load font!" << std::endl;
            return;
        }

       
        sf::Text inputText("", font, 30);
        inputText.setPosition(100, 100);
        inputText.setFillColor(sf::Color::Black);

        std::string playerName = ""; 

        while (window.isOpen())
         {
            sf::Event event;
            while (window.pollEvent(event))
             {
               
                if (event.type == sf::Event::Closed)
                 {
                    window.close();
                }

               
                if (event.type == sf::Event::TextEntered)
                 {
                    if (event.text.unicode < 128) 
                    { // ASCII characters only
                        if (event.text.unicode == '\b') 
                        { // Backspace
                            if (!playerName.empty())
                             {
                                playerName.pop_back();
                            }
                        } else if (event.text.unicode == '\r') 
                        { // Enter key
                            name = playerName; // Save name to the Player object
                            cout << "Player Name: " << name << std::endl;
                            window.close();
                        } 
                        else
                         {
                            playerName += static_cast<char>(event.text.unicode);
                        }
                    }
                }
            }

            // Update the text to display the current input
            inputText.setString("Enter Name: " + playerName);

            // Clear and draw everything
            window.clear(sf::Color::White);
            window.draw(inputText);
            window.display();
        }
    }


    void storePlayerData() {
        std::ofstream outFile("players_data.txt", ios::app); 
        if (outFile.is_open())
         {
           
            outFile << name << "," << player_snake_score << "," << player_wordle_score << "," << player_hangman_score << std::endl;
            outFile.close(); 
            cout << "Player data saved!" << std::endl;
        } 
        else
         {
            cout << "Unable to open file for saving data!" << std::endl;
        }
    }

    string getplayername()
    {
        return name;
    }



    void setplayersnakescore(int sscore)
    {
        player_snake_score=score;
         storePlayerData();
    
   
    }
    

    void setplayerhangmanscore(int hscore)
    {
        
       player_hangman_score=hscore;
        storePlayerData();
    }

    void setplayerwordlescore(int wscore)
    {
        
      player_wordle_score=wscore;
       storePlayerData();
    
    }

};






/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
class sound_system
{
protected:
	Sound sfx;

public:
	virtual void play_sfx(int sfx_id) = 0;
	virtual void play_bg_music() = 0;
	virtual void stop_bg_music() = 0;
	virtual ~sound_system() = default; // use default destructor as sfml will handle clean up of SoundBuffer and Music
};


class hangman_sound : public sound_system {
    SoundBuffer win, loose, right, wrong, tile_sfx;
    Music bgMusic;

public:
    hangman_sound() {
        if (!win.loadFromFile("sfx/hm win.wav"))
            cout << "Failed to load win sound!" << endl;
        if (!loose.loadFromFile("sfx/hm loose.wav"))
            cout << "Failed to load loose sound!" << endl;
        if (!right.loadFromFile("sfx/hm right.wav"))
            cout << "Failed to load right sound!" << endl;
        if (!wrong.loadFromFile("sfx/hm wrong.wav"))
            cout << "Failed to load wrong sound!" << endl;
        if (!tile_sfx.loadFromFile("sfx/wordle tile select.wav"))
            cout << "Failed to load tile sound!" << endl;
       // if (!bgMusic.openFromFile("sfx/background_music.ogg"))
         //   cout << "Failed to load background music!" << endl;
    }

    void play_sfx(int sfx_id) override {
        switch (sfx_id) {
            case 1: sfx.setBuffer(win); break;
            case 2: sfx.setBuffer(loose); break;
            case 3: sfx.setBuffer(right); break;
            case 4: sfx.setBuffer(wrong); break;
            case 5: sfx.setBuffer(tile_sfx); break;
            default: cout << "Invalid SFX ID!" << endl; return;
        }
        sfx.setVolume(100);
        sfx.play();
    }

    void play_bg_music() override {
        bgMusic.setLoop(true);
        bgMusic.setVolume(50);
        bgMusic.play();
    }

    void stop_bg_music() override {
        bgMusic.stop();
    }
};
*/

//class for category selecion
class category {
private:
    char* name;             
    char** words;            
    int word_count;         
public:
    // Default constructor 
    category() {
        name= nullptr;
        words= nullptr;
        word_count=0; 
    }

    // Parameterized constructor 
    category(const char* n, const char** word, int word_count) {
        if (n == nullptr || word == nullptr || word_count <= 0) {
            // for handling invlid input
            this->name = nullptr;
            this->words = nullptr;
            this->word_count = 0;
            return;
        }

        //initializing the name
        this->name = new char[100 + 1];
        strcpy(this->name, n);

        // initializing words
        this->word_count = word_count;
        this->words = new char*[word_count];
        for (int i = 0; i < word_count; i++) {
            this->words[i] = new char[100 + 1];
            strcpy(this->words[i], word[i]);
        }
    }

    // Getter for category name
    const char* get_name() const {
        return name ;
    }

    // Getter for a random word from the category
    const char* get_random() const {
        if (word_count == 0 || words == nullptr) {
            return nullptr; // No words available
        }
        int randomIndex = rand() % word_count;
        return words[randomIndex];
    }

    // Destructor to clean up allocated memory
    ~category() {
        delete[] name; // Free memory for the name
        if (words != nullptr) {
            for (int i = 0; i < word_count; i++) {
                delete[] words[i]; // Free memory for each word
            }
            delete[] words; // Free the array of word pointers
        }
    }
};


//class for keyboard buttons 
class KeyButton {
public:
    sf::CircleShape shape;
    sf::Text text;
    char letter;
    bool isClicked = false;

    KeyButton(float x, float y, char letter, sf::Font &font) : letter(letter) {
    shape.setRadius(25.f);
    shape.setPosition(x - shape.getRadius(), y - shape.getRadius());
    shape.setFillColor(sf::Color(240, 240, 240));
    shape.setOutlineColor(sf::Color(193, 154, 107)); // Light camel brown for rough edges
    shape.setOutlineThickness(2);
    string letterStr(1, letter);
    if (letter != '\0') {  
        text.setFont(font);
        text.setString(letterStr);  
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        text.setStyle(sf::Text::Bold);
        text.setPosition(x - 12.f, y - 12.f);
    } 
}


    bool isPressed(sf::Vector2i mousePos) {
    if (!isClicked && shape.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
        isClicked = true; 
        shape.setFillColor(sf::Color(150, 150, 150));  
        return true;
    }
    return false;
}

};

//class for the building of hangman 
class Progress {
private:
    sf::CircleShape head;           
    sf::RectangleShape body;  
    sf::RectangleShape leftArm, rightArm; // Arms
    sf::RectangleShape leftLeg, rightLeg; // Legs

    int stage; 

public:
 Progress() : stage(0) {
    // Initializing head
    head.setRadius(20.f);
    head.setFillColor(sf::Color::Transparent);
    head.setOutlineColor(sf::Color::Black);
    head.setOutlineThickness(7.f);
    head.setPosition(245.f, 200.f);

    // Initializing body
    body.setSize(sf::Vector2f(6.f, 100.f));
    //body.setOutlineThickness(4.f);
    body.setFillColor(sf::Color::Black);
    body.setPosition(265.f, 240.f);

    // Initializing left arm
    leftArm.setSize(sf::Vector2f(50.f, 6.f));
    leftArm.setFillColor(sf::Color::Black);
    leftArm.setPosition(270.f, 260.f);
    leftArm.setRotation(135.f);

    // Initializing right arm
    rightArm.setSize(sf::Vector2f(50.f, 6.f));
    rightArm.setFillColor(sf::Color::Black);
    rightArm.setPosition(270.f, 260.f);
    rightArm.setRotation(45.f);

    // Initializing left leg
    leftLeg.setSize(sf::Vector2f(50.f, 6.f));
    leftLeg.setFillColor(sf::Color::Black);
    leftLeg.setPosition(270, 340);
    leftLeg.setRotation(135.f);

    // Initializing right leg
    rightLeg.setSize(sf::Vector2f(50.f, 6.f));
    rightLeg.setFillColor(sf::Color::Black);
    rightLeg.setPosition(270, 340);
    rightLeg.setRotation(45.f);
}

    void incrementStage() {
        if (stage < 6) {
            stage++;
        }
    }
     int getWrongGuesses() const {
        return stage;
    }

    void reset() {
        stage = 0;
    }

    


    void draw(sf::RenderWindow& window) {
        if (stage >= 1) 
        {
            window.draw(head);

        }
        if (stage >= 2){
            window.draw(body);
        }
        if (stage >= 3)
        {
            window.draw(leftArm);
            
        }
        if (stage >= 4)
        {
            window.draw(rightArm);
        }
        if (stage >= 5) {
            window.draw(leftLeg);
        }
        if (stage >= 6) {
            window.draw(rightLeg);
        }
    }
};

//class for timing
class Timer{
private:
    int start_time;
    int duration;
    bool is_runing;
    sf::Font font;
    sf::Text timer_text;
    int last_displayed_time;
 

public:
    Timer(){
        duration=0;
        is_runing=false;
        start_time=0;
        last_displayed_time = -1;
    }
    Timer(int d, const sf::Font& font){
        duration = d;
        this->font  = font;
        timer_text.setFont(this->font);
        timer_text.setCharacterSize(30);
        timer_text.setFillColor(sf::Color::Black);
        timer_text.setStyle(sf::Text::Bold);
        timer_text.setPosition(600,50);
    }
    void start_timer(){
        is_runing= true;
        start_time= time(nullptr);

    }
    void stop_timer(){
        is_runing =  false;

    }
    bool run_out() const{
        if(is_runing == false){
            return false;
        }
    int current_time;
    int elapsed;
    current_time= time(0);
    elapsed= current_time - start_time;
        if(elapsed >= duration){
            return true;
        }
        else{
            return false;
        }
    }
    void updating_time(){
        if(!is_runing){
            return ;
        }
        
        int current_time;
        int elapsed;
        int remaining_time;
        current_time= time(0);
        last_displayed_time = -1;
        elapsed= current_time - start_time;
        remaining_time= duration- elapsed ;

        if(remaining_time <=0){
            remaining_time = 0;
        }

        if (remaining_time != last_displayed_time) {
            last_displayed_time = remaining_time;
            timer_text.setString("Time: " + to_string(remaining_time));
        }
    }

    void draw_time(sf::RenderWindow& window){
        window.draw(timer_text);
    }
};
//hint class 
class Hint {
private:
    sf::Font font;
    sf::Text hintButtonText;
    sf::RectangleShape hintButton;
    sf::Text noHintsText;
    int hintsLeft;

public:
    Hint(){
    hintsLeft= 2;
    }

    void initialize() {
        if (!font.loadFromFile("Roboto-Regular.ttf")) {
            cout << "Font not loading" << endl;
        }

        // Button for hint
        hintButton.setSize(sf::Vector2f(150.f, 50.f));
        hintButton.setFillColor(sf::Color(245, 245, 220));
        hintButton.setOutlineThickness(2); 
        hintButton.setOutlineColor(sf::Color(153, 101, 21));
        hintButton.setPosition(950.f, 600.f);

        hintButtonText.setFont(font);
        hintButtonText.setCharacterSize(20);
        hintButtonText.setFillColor(sf::Color::Black);
        hintButtonText.setString("Hint");
        hintButtonText.setStyle(sf::Text::Bold);
        hintButtonText.setPosition(990,610);

        // Text for no hints left
        noHintsText.setFont(font);
        noHintsText.setCharacterSize(20);
        noHintsText.setFillColor(sf::Color::Red);
        noHintsText.setStyle(sf::Text::Bold);
        noHintsText.setString("");
        noHintsText.setPosition(950,650);
    }

    bool handleClick(sf::Vector2i mousePosition) {
        // Checking if button is clicked
        if (hintButton.getGlobalBounds().contains(sf::Vector2f(mousePosition))) {
            if (hintsLeft > 0) {
                hintsLeft--;
                return true; // Hint used
            } else {
                noHintsText.setString("No hints left!");
            }
        }
        return false; // No hint available
    }

    void draw(sf::RenderWindow& window) {
        // Drawing the button and its text
        window.draw(hintButton);
        //button text
        window.draw(hintButtonText);

        // Drawing "No hints left" message if applicable
        window.draw(noHintsText);
    }

    int getHintsLeft() const {
        return hintsLeft;
    }
};
//high score class
class hangman_HighScore {
private:
    int score;

public:
    hangman_HighScore() {
        score =0;
    }
    void increaseScore() {
        score += 10; //inc 10 at each right guess
    }

    // get the current score
    int getScore() const {
        return score;
    }

    //to reset score 
    void resetScore() {
        score = 0;
    }

    //display the score
    void displayScore(sf::RenderWindow& window, sf::Font& font) {
        sf::Text scoreText("Score: " + to_string(score), font, 30);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition(990.f, 50.f); 
        scoreText.setStyle(sf::Text::Bold);
        window.draw(scoreText);
    }
};


//game base class
class HangmanGame {
private:
    sf::RenderWindow window;

    sf::Texture screen2_Texture;
    sf::Sprite screen2_Sprite;

    sf::Texture screen1_Texture;
    sf::Sprite screen1_Sprite;

    sf::Font font;
    sf::RectangleShape basePole;
    sf::RectangleShape verticalPole;
    sf::RectangleShape topBar;
    sf::RectangleShape hangingBar;
    sf::Text endMessage;

    category* categories[200];
    int categoryCount;
    int isGameOver;
    const char* selected_cat;
    char selectedWord[100];
    char guessedWord[100];
    //int wrongGuesses = 0;

    KeyButton* keys[26];  // Array for keyboard keys
    Progress progress;
    Timer* timer;
    Hint hint;
    hangman_HighScore highscore;
    //hangman_sound sound;
public:

    
    HangmanGame() : window(sf::VideoMode(800, 600), "Hangman Game")
     {
        
        if (!font.loadFromFile("Roboto-Regular.ttf")) {
            cout << "fond not loading" << endl;
            
        }
        if (screen2_Texture.loadFromFile("image.jpeg")) { 
            cout << "Background image loading" << endl;
        }
        screen2_Sprite.setTexture(screen2_Texture);
        handleResize(window,screen2_Sprite, screen2_Texture ,true);

    

        srand((time(0)));

        //selectedCategory;
        selectedWord[0] = '\0';
        guessedWord[0] = '\0';
        categoryCount=0;
        isGameOver=0;
        progress = Progress(); 
        timer =  new Timer(60, font);
        hint.initialize();

        for (int i = 0; i < 200; i++) {
        categories[i] = nullptr;  
        }

        // Initializing keys array for the keybooard
        for (int i = 0; i < 26; i++) {
            keys[i] = nullptr;
        }

        basePole.setSize(sf::Vector2f(150.f, 10.f));
        basePole.setFillColor(sf::Color::Black);
        basePole.setPosition(20.f, 540.f); 

        verticalPole.setSize(sf::Vector2f(10.f, 400.f));
        verticalPole.setFillColor(sf::Color::Black);
        verticalPole.setPosition(70.f, 140.f); 

        topBar.setSize(sf::Vector2f(200.f, 10.f));
        topBar.setFillColor(sf::Color::Black);
        topBar.setPosition(70.f, 140.f); 
        hangingBar.setSize(sf::Vector2f(10.f, 53.f));
        hangingBar.setFillColor(sf::Color::Black);
        hangingBar.setPosition(260.f, 140.f);


        endMessage.setFont(font);
        endMessage.setCharacterSize(30);
        endMessage.setFillColor(sf::Color::Black);
        endMessage.setStyle(sf::Text::Bold);
        endMessage.setPosition(420.f, 400.f); 
        endMessage.setString("");
      
        for (int i = 0; i < 26; i++) { //as there ar 26 keys
        float xPos = 400.f + (i % 10) * 55.f;  //x position
        float yPos = 200.f + (i / 10) * 55.f;  //y position
        keys[i] = new KeyButton(xPos, yPos, 'A' + i, font);  // Allocateing the memory for the keys
    }
        
        virtual_Keyboard();
        all_Categories();  
}

    ~HangmanGame() {
    for (int i = 0; i < categoryCount; i++) {
        if (categories[i]) {
            delete categories[i];
        }
    }
    for (int i = 0; i < 26; i++) {
        if (keys[i]) {
            delete keys[i];
        }
    }
}


    void handleResize(sf::RenderWindow& window, sf::Sprite& backgroundSprite, const sf::Texture& backgroundTexture, bool hasBackgroundImage) {
    sf::View view = window.getDefaultView();
    view.setSize(window.getSize().x, window.getSize().y);
    view.setCenter(window.getSize().x / 2.f, window.getSize().y / 2.f);
    window.setView(view);


    if (hasBackgroundImage) {
        float scaleX = float(window.getSize().x) / backgroundTexture.getSize().x;
        float scaleY =float(window.getSize().y) / backgroundTexture.getSize().y;

   

        // Apply the scaling to the sprite
        backgroundSprite.setScale(scaleX, scaleY);

        // Center the sprite if necessary
        backgroundSprite.setPosition(0, 0);
    }
}
    void virtual_Keyboard() {
   //statring position for the keys
    float xPos = 400.f, yPos = 200.f; 
    //spaces
    float xStep = 55.f, yStep = 55.f;

 
    for (int i = 0; i < 26; i++) {
        float adjustedX = xPos + (i % 10) * xStep;
        float adjustedY = yPos + (i / 10) * yStep;
        
        keys[i]->shape.setPosition(adjustedX, adjustedY);
        //placing at the middle of keys the text
        keys[i]->text.setPosition(adjustedX + 12.f, adjustedY + 12.f); 
    }

}


  


    void all_Categories() {
    // Initializing the word lists
    const char* animals[] = {"Dog", "Cat", "Horse", "Tiger", "Lion","Polar bear", "Turtle", "Tortoise", "Crocodile","Rabbit", "Porcupine", "Hare", "Hen","Pigeon", "Albatross", "Crow", "Fish",
        "Dolphin", "Frog", "Whale", "Alligator","Eagle", "Flying squirrel", "Ostrich", "Fox","Goat", "Jackal", "Emu", "Armadillo",
        "Eel", "Goose", "Arctic fox", "Wolf","Beagle", "Gorilla", "Chimpanzee", "Monkey","Beaver", "Orangutan", "Antelope", "Bat",
        "Badger", "Giraffe", "Hermit Crab", "Giant Panda","Hamster", "Cobra", "Hammerhead shark", "Camel",
        "Hawk", "Deer", "Chameleon", "Hippopotamus","Jaguar", "Chihuahua", "King Cobra", "Ibex","Lizard", "Koala", "Kangaroo", "Iguana",
        "Llama", "Chinchillas", "Dodo", "Jellyfish","Rhinoceros", "Hedgehog", "Zebra", "Possum","Wombat", "Bison", "Bull", "Buffalo",
        "Sheep", "Meerkat", "Mouse", "Otter","Sloth", "Owl", "Vulture", "Flamingo","Racoon", "Mole", "Duck", "Swan",
        "Lynx", "Monitor lizard", "Elk", "Boar","Lemur", "Mule", "Baboon", "Mammoth","Blue whale", "Rat", "Snake", "Peacock"};

    const char* foods[] = {"Pizza", "Burger", "Pasta", "Apple", "Salad","Steak", "Tuna Steak", "Fish", "Shrimp", "Rice", "Spaghetti", 
        "Eggs", "Cheese", "Sausages", "Apple Juice", "Grape Juice", "Milk", 
        "Candy", "Cookie", "Pie", "Cake", "Cupcake"};

    const char* colors[] = {"Red", "Blue", "Green", "Orange", "Yellow","Gray", "Pink", "Olive", "Maroon", "Violet", "Charcoal", "Magenta", "Bronze", "Cream", "Gold",
        "Tan", "Teal", "Mustard", "Navy Blue", "Coral", "Burgundy", "Lavender", "Mauve", "Cyan", "Peach",
        "Rust", "Indigo", "Ruby", "Lime Green", "Salmon", "Azure", "Beige", "Copper Rose", "Turquoise", "Aqua",
        "Mint", "Sky Blue", "Crimson", "Saffron", "Lemon Yellow", "Grapevine", "Fuschia", "Amber", "Sea Green",
        "Dark Green", "Pearl", "Ivory", "Tangerine", "Garnet", "Cherry Red", "Emerald", "Brunette", "Sapphire",
        "Lilac", "Rosewood", "Arctic Blue", "Ash", "Mocha", "Coffee Brown", "Jet Black"};

    const char* sports[] = {"Parkour", "Capoeira", "Lacrosse", "Snooker", "Kendo", "soccer", "basketball", "tennis", "baseball", "golf", "running", "volleyball", "badminton", "swimming",
        "boxing", "table tennis", "skiing", "ice skating", "roller skating", "cricket", "rugby", "pool", "darts",
        "football", "bowling", "ice hockey", "surfing", "karate", "horse racing", "snowboarding", "skateboarding",
        "cycling", "archery", "fishing", "gymnastics", "figure skating", "rock climbing", "sumo wrestling", "taekwondo",
        "fencing", "water skiing", "jet skiing", "weight lifting", "scuba diving", "judo", "wind surfing", "kickboxing",
        "sky diving", "hang gliding", "bungee jumping", "lacrosse", "polo", "wrestling", "squash", "handball", "rowing",
        "sailing", "kitesurfing", "triathlon", "paddleboarding", "ultimate frisbee", "hiking", "canoeing/kayaking",
        "cross-country skiing", "mixed martial arts", "paragliding", "equestrian ",
        "synchronized Swimming", "snowmobiling", "biathlon", "curling", "racquetball",
};

    
    int animalCount = sizeof(animals) / sizeof(animals[0]);
    int foodCount = sizeof(foods) / sizeof(foods[0]);
    int colorCount = sizeof(colors) / sizeof(colors[0]);
    int sportCount = sizeof(sports) / sizeof(sports[0]);

    if (categoryCount < 200) {
        categories[categoryCount++] = new category("Animals", animals, animalCount);
    }
    if (categoryCount < 200) {
        categories[categoryCount++] = new category("Foods", foods, foodCount);
    }
    if (categoryCount < 200) {
        categories[categoryCount++] = new category("Colors", colors, colorCount);
    }
    if (categoryCount < 200) {
        categories[categoryCount++] = new category("Sports", sports, sportCount);
    }
  
}

void displayCategories(sf::RenderWindow& window, sf::Font& font, HangmanGame& game) {

     if (!screen1_Texture.loadFromFile("image3.jpeg")) {
        cout << "Background image not loaded" << endl;
        }
        screen1_Sprite.setTexture(screen1_Texture);
   
    if (!font.loadFromFile("Roboto-Regular.ttf")) {
        cout << "Font not loading";
        return;
    }
    
    handleResize(window,screen1_Sprite, screen1_Texture ,true);

    while (window.isOpen()) {

        sf::Event event;
        window.clear(sf::Color::White);
        window.draw(screen1_Sprite);
       
       
        sf::Text title("Welcome to Hangman Game", font, 50);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(sf::Color::Black);
        title.setPosition(50, 50);
        window.draw(title);

        sf::Text asking("Select a Category:", font, 30);
        asking.setStyle(sf::Text::Bold);
        asking.setFillColor(sf::Color::Black);
        asking.setPosition(70, 160);
        window.draw(asking);

     
        sf::Text labels[4];
        const char* cat_name[] = {"Animals", "Foods", "Colors", "Sports"};

        sf::Vector2f buttonSize(300, 50); // Width and height of the button

        for (int i = 0; i < 4; i++) {
            
            sf::RectangleShape button(sf::Vector2f(buttonSize.x, buttonSize.y)); //buttons for the category 
            //shape for button is rectnagle 
            //setting the position for the buttons
            button.setPosition(50, 220 + i * 70);  
            button.setFillColor(sf::Color(245, 245, 220));
            button.setOutlineThickness(2);
            button.setOutlineColor(sf::Color(101, 67, 33));

         
            labels[i].setFont(font);
            labels[i].setString(cat_name[i]);
            labels[i].setCharacterSize(24);
            labels[i].setFillColor(sf::Color(101, 67, 33));
            labels[i].setPosition(60, 230 + i * 70); 

            // Ensure text is within bounds of button (width)
            if (labels[i].getGlobalBounds().width > button.getGlobalBounds().width) {
                labels[i].setCharacterSize(20);  // Adjust text size if too large
            }

            // Draw the button and the label
            window.draw(button);
            window.draw(labels[i]);
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
            

                handleResize(window,screen1_Sprite, screen1_Texture ,true);
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);


                for (int i = 0; i < 4; i++) {
                    sf::FloatRect bounds(50, 220 + i * 70, buttonSize.x, buttonSize.y);
                    if (bounds.contains(mousePos.x, mousePos.y)) { //checking either the bounds we set for the button contains mouse or not
                      
                        switch (i) {
                            case 0:
                            game.selectCategory("Animals"); 
                            //sound.play_sfx(5);
                            break;
                            case 1:
                             game.selectCategory("Foods");
                            // sound.play_sfx(5);
                            break;
                            case 2: 
                            game.selectCategory("Colors"); 
                           // sound.play_sfx(5);
                            break;
                            case 3: 
                            game.selectCategory("Sports"); 
                           // sound.play_sfx(5);
                            break;
                            default: 
                            break;
                        }
                        return; 
                    }
                }
            }
        }

        window.display();
        
    }
}

void selectCategory(const char* categoryName) {
    //error checking
    if (categoryName == nullptr) {
       return;
    }

  
    for (int i = 0; i < categoryCount; i++) {
      
        if (categories[i] == nullptr) {
            return; 
        }

        const char* current_category = categories[i]->get_name();
        if (current_category == nullptr) {
            return;  
        }

 
        if (strcmp(current_category, categoryName) == 0) {
            selected_cat = current_category;  

            const char* randomWord = categories[i]->get_random();
               
            if (randomWord == nullptr) {
                return;
            }

           
            int wordLength = 0;
            while (randomWord[wordLength] != '\0') {
                selectedWord[wordLength] = randomWord[wordLength]; //copying the random word selected from category to the selected
                wordLength++;
            }
            selectedWord[wordLength] = '\0';  // Null-terminate selectedWord

            // Initialzing the guess word length
            int guessedWordLength = wordLength;
            for (int j = 0; j < guessedWordLength; j++) {
                guessedWord[j] = '_';
            }
            guessedWord[guessedWordLength] = '\0';  // Null-terminate guessedWord

            return;
        }
    }
 
}
   void playGame() {

    
    timer->start_timer();

    while (window.isOpen()) {
        sf::Event event;
        window.clear(sf::Color::White);
        window.draw(screen2_Sprite);
        handleResize(window,screen2_Sprite, screen2_Texture ,true);

        drawGameScreen();
  
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

                
            timer->updating_time();

            if (timer->run_out()) {
            isGameOver = 3;
            //sound.play_sfx(2);
            endMessage.setString("Time's up! The correct word was: " + string(selectedWord));;
            //window.close(); 
            return;
        }

            timer->draw_time(window);
               
            if (event.type == sf::Event::Resized) {
              
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                handleResize(window,screen2_Sprite, screen2_Texture ,true);
                
                float xPos = event.size.width / 4.0f;
                float yPos = event.size.height / 2.0f;
                float xStep = 55.f, yStep = 55.f;

                keys[0]->shape.setPosition(xPos, yPos);
                for (int i = 1; i < 26; i++) {
                    float adjustedX = xPos + (i % 10) * xStep;
                    float adjustedY = yPos + (i / 10) * yStep;
                    keys[i]->shape.setPosition(adjustedX, adjustedY);
                    keys[i]->text.setPosition(adjustedX + 12.f, adjustedY + 12.f);   
                }
            }
            

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (hint.handleClick(mousePos)) {
                //giving a hint
                for (int i = 0; selectedWord[i] != '\0'; i++) {
                    if (guessedWord[i] == '_') {  
                        guessedWord[i] = selectedWord[i];
                        break;
                    }
                }
            }
                for (int i = 0; i < 26; i++) {
                    if (keys[i]->isPressed(mousePos)) {
                        processGuess(keys[i]->letter);
                        break;
                    }
                }
            }
        }

        window.display();
    }

}


 void drawGameScreen() {
   // window.clear();  // Clearing frames
   // window.clear(sf::Color::White);
    window.draw(screen2_Sprite);

    // Drawing the rod
    window.draw(basePole);
    window.draw(verticalPole);
    window.draw(topBar);
    window.draw(hangingBar);

    progress.draw(window);
    timer->draw_time(window);
    hint.draw(window);
     

    if (selected_cat == nullptr) {
        selected_cat = "Unknown ";  
    }
    sf::Text categoryText("Category: " + string(selected_cat), font, 30);
    categoryText.setStyle(sf::Text::Bold);
    categoryText.setFillColor(sf::Color::Black);
    categoryText.setPosition(50, 50);
    window.draw(categoryText);
   

    string dashes;
    for (int i = 0; guessedWord[i] != '\0'; i++) {
        if (guessedWord[i] == '_') {
            dashes += "_ ";  
        } else {
            dashes += guessedWord[i];  
            dashes += " ";
        }
    }

 
    sf::Text dashesText(dashes, font, 40);
    dashesText.setFillColor(sf::Color(240, 240, 240));
    dashesText.setOutlineColor(sf::Color(169, 169, 169));
    dashesText.setOutlineThickness(1);
    dashesText.setPosition((window.getSize().x - dashesText.getGlobalBounds().width) / 2, window.getSize().y - 100);
    window.draw(dashesText);

    highscore.displayScore(window, font);


    sf::Text hintsLeftText("Hints left: " + to_string(hint.getHintsLeft()), font, 20);
    hintsLeftText.setFillColor(sf::Color::Black);
    hintsLeftText.setStyle(sf::Text::Bold);;
    hintsLeftText.setPosition(950, 570);
    window.draw(hintsLeftText);


    
    float xStart = 600;   //starting X position for keys
    float yStart = 200; //starting Y position 
    float xStep = 55;  //spacing
    float yStep = 55;
    int keysPerRow = 10;

    for (int i = 0; i < 26; i++) {
        float adjustedX = xStart + (i % keysPerRow) * xStep;
        float adjustedY = yStart + (i / keysPerRow) * yStep;

        keys[i]->shape.setPosition(adjustedX - keys[i]->shape.getRadius(), adjustedY - keys[i]->shape.getRadius());
        keys[i]->text.setPosition(adjustedX - 12.f, adjustedY - 12.f);

        window.draw(keys[i]->shape);
        window.draw(keys[i]->text);
    }
    if (isGameOver != 0) {  // 1 for win, 2 for loss, 3 for time up 
        window.draw(endMessage);
    }

    window.display(); 
}



    void processGuess(char letter) {
    bool correct = false;

    //
    for (int i = 0; selectedWord[i] != '\0'; i++) {
         if (selectedWord[i] == letter || 
            (selectedWord[i] >= 'A' && selectedWord[i] <= 'Z' && selectedWord[i] + 32 == letter) ||
            (selectedWord[i] >= 'a' && selectedWord[i] <= 'z' && selectedWord[i] - 32 == letter)) {
            
            guessedWord[i] = selectedWord[i];
            correct = true;
        }
    }

    if (correct) {
        //sound.play_sfx(3);
        highscore.increaseScore();  // Increase score by 10 for correct guess
    }
    
    if (!correct) {
       // sound.play_sfx(4);
        progress.incrementStage();  
        progress.draw(window);
    }


     if (strcmp(guessedWord, selectedWord) == 0) {
       // cout << "You Win!" << endl;
        isGameOver = 1; 
        //sound.play_sfx(1);
        endMessage.setString("You won!");
       // window.close();
    } else if (progress.getWrongGuesses() == 6) {
       // cout << "You Lose! The word was: " << selectedWord << endl;
        isGameOver = 2; 
        //sound.play_sfx(2);
        endMessage.setString("You Lost! The correct word was: " + string(selectedWord));
       // window.close();
    }
    //window.draw(endMessage);

    for (int i = 0; i < 26; i++) {
        if (keys[i]->letter == letter) {
            keys[i]->isClicked = false;  // Reseting the key click status
        }
    }
}


    void run() {
        displayCategories(window, font, *this);
        playGame();
    }
};

























//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Highscore {
    int HS;
    int scorepergame;

public:
    Highscore() : HS(0) {
        loadHighScore();
    }

   
    void loadHighScore() 
    {
        ifstream scorefile("SnakeHighScore.txt");
        ifstream file("snakescorepergame.txt");

        if (file.is_open())
         {
            file >> HS;
            file.close();
        } 
        else 
        {
            cout << "Error! Unable to load the high score file!" << endl;
            HS = 0; 
        }
    }

    
    void savenewHS() 
    {
        ofstream file("HighScore.txt");
        if (file.is_open())
         {
            file << HS;
            file.close();
        } 
        else
         {
            cout << "Error! Unable to save the high score file!" << endl;
        }
    }

    
    void updateHS(int score) 
    {
        if (score > HS)
         {
            HS = score;
            savenewHS();
        }
    }


    void addscorepergame(int score)
    {
      ofstream scorefile("snakescorepergame.txt", ios::app);
        if (scorefile.is_open())
         {
            scorefile<< score<<endl;
            scorefile.close();
        } 
        else
         {
            cout << "Error!...Unable to save the score file" << endl;
        }
    }

    int gethighscore() const 
    {
        return HS;
    }
};

struct GameState {

    int score;
    int snakeLength;
    int snakePositionX[100]; // Maximum length of the snake
    int snakePositionY[100];
    int foodX;
    int foodY;
    sf::Vector2i direction;


    
};




class Position {
    int posX, posY;

public:
    Position(int x = 0, int y = 0) : posX(x), posY(y) {}

    bool operator==(const Position& other) const
    {
        return posX == other.posX && posY == other.posY;
    }

    int getX() const
     { return posX; }
     

    int getY() const
     { 
        return posY; 
    }


    void setPosition(int x, int y)
    {
        posX = x;
        posY = y;
    }

    void getrandposition()
     {
        posX = (rand() % (maxWidth / 30)) * 30;
        posY = (rand() % (maxHeight / 30)) * 30;
    }
};





class Hurdles{
    
Position Hurdlepos;
sf::RectangleShape hurdle;
sf::Sprite Hsprite;
sf::Texture Htexture;

public:

Hurdles(){

     hurdle.setSize(sf::Vector2f(40.0f, 20.0f));
     Hurdlepos.getrandposition();
     hurdle.setPosition(Hurdlepos.getX(), Hurdlepos.getY());
     hurdle.setFillColor(sf::Color::Cyan);



     if (!Htexture.loadFromFile("hurdleimg.png")) 
     {
            cout << "Error loading hurdle image!" << endl;
        }


       Hsprite.setTexture(Htexture);
        Hurdlepos.getrandposition();


        float hurdleX=0.2;
        float hurdleY =0.2;

        Hsprite.setScale(hurdleX, hurdleY);
        Hsprite.setPosition(Hurdlepos.getX(), Hurdlepos.getY());

}



    void makehurdle(sf::RenderWindow& window) 
    {
        //window.draw(hurdle);
        window.draw(Hsprite);
    }


    Position getPosition() const 
    { return Hurdlepos; }




};



class Food {
    Position position;
    sf::CircleShape cookie;
    sf::Sprite fsprite;
    sf::Texture foodTexture;

public:
    Food() {

      if (!foodTexture.loadFromFile("appleimg.png")) 
      {
            cout << "Error loading food image!" << endl;
            
        }


       fsprite.setTexture(foodTexture);
        fsprite.setPosition(position.getX(), position.getY());
        position.getrandposition();
       
        float foodX=0.05;
        float foodY =0.05;

        fsprite.setScale(foodX, foodX);
        fsprite.setPosition(position.getX(), position.getY());

    }

    void updateposition()
     {
        position.getrandposition();
        fsprite.setPosition(position.getX(), position.getY());
     }

    void makefood(sf::RenderWindow& window) 
    {
        window.draw(fsprite);
    }


    Position getPosition() const 
    { return position; }


};




class PowerFood:public Food{


    Position position;
    sf::Sprite PFSprite;   
    sf::Texture PFtexture;
    sf::Clock visibilityTimer;
    sf::Clock cooldownTimer; 
    bool activated;
    

public:
    PowerFood() : activated(true) {




        if (! PFtexture.loadFromFile("candynobg.png"))
         {
           cout << "Error loading power food texture!" << endl;
         } 

        PFSprite.setTexture( PFtexture);
        position.getrandposition();
         PFSprite.setPosition(position.getX(), position.getY());
         

      
        float x = 0.03f;
        float y = 0.03f;

       PFSprite.setScale(x, y);
        // powerSprite.setPosition(position.getX(), position.getY());


        
       

        visibilityTimer.restart(); 


    }

    void updateposition()
     {
        if (activated) 
        {

            position.getrandposition();
           PFSprite.setPosition(position.getX(), position.getY());

        }
    }

    void draw(sf::RenderWindow& window)
     {
        if (isVisible())
         {
            window.draw(PFSprite);
        }

    }

    bool isVisible() const 
    {
       
        return activated && visibilityTimer.getElapsedTime().asSeconds() < 10.0f;
    }

    void update() 
    {



        if (activated && visibilityTimer.getElapsedTime().asSeconds() >= 10.0f)
         {
            
            deactivate();
        }

        if (!activated && cooldownTimer.getElapsedTime().asSeconds() >= 10.0f)
         {
            
            activate();
            position.getrandposition(); 
            PFSprite.setPosition(position.getX(), position.getY());
        }



    }

     //checks the collision between powerfood and snake only when the power food is activated
     bool checkCollision(const Position& snakehead, const Position& foodpos, int csize, int x, int y)
      {
        //return activated && position == snakehead;

       int headx =  snakehead.getX();
        int heady =  snakehead.getY();

   
        int PFx = foodpos.getX();
        int PFy = foodpos.getY();

        if ((headx + csize >  PFx && headx - csize <  PFx + x && heady + csize >PFy && heady - csize < PFy + y) && activated)
        {
            return true;
        }
        return false;


    }
    


    void activate()
     {
        activated = true;
        visibilityTimer.restart();
    }

    void deactivate() 
    {
        activated = false;
        cooldownTimer.restart();
    }

    Position getPosition() const
     { return position; }




};








class Snakebody {
    Position position;
    sf::Sprite headSprite;
    sf::Texture headTexture;
    player plyr;

public:

    Snakebody() {
        if (!headTexture.loadFromFile("snakehead.png")) 
        {
            cout << "Error loading head image!" << endl;
        }

        headSprite.setTexture(headTexture);


        float headsize = 33.0f;
        sf::Vector2u textureSize = headTexture.getSize();
        float scaleX = headsize/ textureSize.x;
        float scaleY = headsize/ textureSize.y;
        headSprite.setScale(scaleX, scaleY);

        headSprite.setPosition(position.getX(), position.getY());
    }



   
    sf::Sprite getSprite() const 
    {
        return headSprite;
    }

    void setPosition(int x, int y)
     {
        headSprite.setPosition(x, y);
    }


};



class Snake {
    Position head;
    int tailX[MAX_TAIL_SIZE];
    int tailY[MAX_TAIL_SIZE];
    player plyr;

    
    int snakesize;
    int speed;
    int level;
    Snakebody snakebody;
    sf::Vector2i direction;
    sf::CircleShape snakehead;

public:
   Snake() : snakesize(2), speed(0), direction(1, 0) 
   {
    
       tailX[0] = head.getX(); 
       tailY[0] = head.getY();
    }






    void setDirection(const sf::Vector2i& newdir)  //to change the direction of movement 
     {
        direction = newdir;
    }


   void setSpeed(int speed2) //changes the speed
   {
       speed = speed2;
    }

    int getspeed() const
   {
      return speed;
    }

    int getsnakesize()
    {
       return snakesize;
    }

     void setsnakesize(int size)
    {
        snakesize=size;

    }

  void move() {
   
    


    // Updating positions of the tail
    for (int i = snakesize - 1; i > 0; --i)
     {
        
        tailX[i] = tailX[i - 1];
        tailY[i] = tailY[i - 1];
    }
     

     int newX = head.getX();
     int newY = head.getY();

       //joining head and the tail by placing head at 0 index of tail
       tailX[0] = newX;
       tailY[0] = newY;


//if will switch the direction of the snake 
    switch (direction.x) 
    {

    case 1: // right
        head.setPosition(newX + 15, newY);
        break;

    case -1: // left
        head.setPosition(newX - 15, newY);
        break;

    case 0: // up or down
        if (direction.y == 1) 
        { // down
            head.setPosition(newX, newY + 15);
        } 
        else
         { // up
            head.setPosition(newX, newY - 15);
        }
        break;
    }





    // Wrap around screen: snake enter one edge appears from other side
    if (head.getX() < 0)                                        // Left to right
     {
        head.setPosition(maxWidth - 30, head.getY());
    }
    if (head.getY() < 0)                                            // Top to bottom
     {
        head.setPosition(head.getX(), maxHeight - 30);
     }
    if (head.getX() >= maxWidth)
     {
        head.setPosition(0, head.getY());                                    // Right to left
    }
    if (head.getY() >= maxHeight)                                            // Bottom to top
     {
        head.setPosition(head.getX(), 0);   
    }

    

}


//function to check if snake collides with his own tail
    bool checkselfcollision()
     {
        for (int i = 1; i < snakesize; ++i) // Skip head
         {
            if (tailX[i] == head.getX() && tailY[i] == head.getY())
             {
                return true;
            }

        }
        return false;
    }




    void grow()     //when snake eats the food it calls grow function which adds another sq shape to end of snake tail
    {
        if (snakesize < MAX_TAIL_SIZE) 
        {

            tailX[snakesize] = tailX[snakesize - 1];
            tailY[snakesize] = tailY[snakesize - 1];
            ++snakesize;


        }
    }
    


    void makesnake(sf::RenderWindow& window)   ///to make the snake tail
     {

       
        snakebody.setPosition(head.getX(), head.getY());

        window.draw(snakebody.getSprite());

        for (int i = 1; i < snakesize; ++i)
         {
            sf::RectangleShape Stail(sf::Vector2f(27, 27));
            Stail.setFillColor(sf::Color::Green);
            Stail.setPosition(tailX[i], tailY[i]);
            window.draw(Stail);
        }
    }


    Position getPosition() const
     { return head; }



};



class SnakeGame {


    sf::RenderWindow window;
    Snake snake;
    Food foods[MAX_FOOD];
    Hurdles hurdles[ MAX_Hurdle];
    Highscore highscore;
    sf::Texture bgtexture;
    sf::Texture splashTexture;
    sf::Sprite splashSprite;
    PowerFood powerFood;
    GameState currentState;
    player plyr;
    Snake snakestate;



public:
   SnakeGame() : window(sf::VideoMode(maxWidth, maxHeight), "Snake Game") {

        
        if (!bgtexture.loadFromFile("darkgrass.jpg")) 
        {
            cout<< "Error loading background image!" << endl;
            return; 
        }


      
      powerFood = PowerFood();
 

      sf::RenderWindow window(sf::VideoMode(800, 600), "Splash Screen");
      unsigned int maxWidth = window.getSize().x;
      unsigned int maxHeight = window.getSize().y;

      splash(window, maxWidth, maxHeight);

    
  


    }
   

//splash is the image displayed when game is started
 void splash(sf::RenderWindow &window, unsigned int w, unsigned int h) 
 {
   
    sf::Texture splashTexture;
    if (!splashTexture.loadFromFile("splashh.jpg"))
     {
        cout << "Error loading splash:" << endl;
        return;
    }

   
    sf::Sprite ssprite;
    ssprite.setTexture(splashTexture);


    sf::Vector2u textureSize = splashTexture.getSize();
    float x = static_cast<float>(w) / textureSize.x;
    float y = static_cast<float>(h) / textureSize.y;
    ssprite.setScale(x, y);

   
    sf::Font font;
    if (!font.loadFromFile("Lobster-Regular.ttf")) 
    {
        cout<< "Error loading font" <<endl;
        return;
    }

    
    sf::Text f;
    f.setFont(font);
    f.setString("Press ENTER to Play");
    f.setCharacterSize(24);
    f.setFillColor(sf::Color::Blue);

    
    sf::FloatRect textBounds = f.getLocalBounds();
    f.setPosition(w / 2 - textBounds.width / 2, h - textBounds.height - 20);

   
    window.clear();
    window.draw(ssprite);
    window.draw(f);
    window.display();

   
    sf::Event event;
    while (window.waitEvent(event)) 
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
            break;
        } 
        else if (event.type == sf::Event::Closed) 
        {
            window.close();
            break;
        }
    }
}

   

    bool checkCollision(const Position& pos1, const Position& pos2)
     {
        return pos1 == pos2;
     }
 
    
    //it checks the collision for hurdle and snake
bool checkCollision(const Position& pos1, const Position& pos2, int csize, int x, int y)
{
    
    int headx = pos1.getX();
    int heady = pos1.getY();

   
    int hurdlex = pos2.getX();
    int hurdley = pos2.getY();

    if (headx + csize > hurdlex && headx - csize < hurdlex + x && heady + csize > hurdley && heady - csize < hurdley + y) 
    {
        return true;
    }
    return false;
}




bool foodcollision()
{
    for (int i = 0; i < MAX_FOOD; ++i)
     {
        //checks the collision between snake and the food Assuming food has a 15 size
        if (checkCollision(snake.getPosition(), foods[i].getPosition(), 15, 30, 30))
         { 
           foods[i].updateposition();
            return true; 
        }
    }
    return false;
}

bool checkPFcollision() 
{
     if(checkCollision(snake.getPosition(), powerFood.getPosition(), 15, 40, 40))
     {   snake.grow();
       
        
        return true;
     }

    else
    {return false;}
}

bool hurdlecollision()
{
    for (int i = 0; i < MAX_Hurdle; ++i) 
    {
        //checks collision between hurdle and snake: hurdle is as 40x20
        if (checkCollision(snake.getPosition(), hurdles[i].getPosition(), 15, 40, 20)) 
        { 
            return true;
        }
    }
    return false;
}


//function to preseve the gamestate
void loadState(GameState &gs) 
{
    ifstream inFile("statepreserve.txt");
gs.snakeLength=snakestate.getsnakesize();
    if (inFile) {
        inFile >> score;
        inFile >> gs.snakeLength;



       // for (int i = 0; i < gs.snakeLength; i++) 
        //{
          //  inFile >> gs.snakePositionX[i] >> gs.snakePositionY[i];
        //}

        //inFile >> gs.foodX >> gs.foodY;
        //int dirX, dirY;
        //inFile >> dirX >> dirY;
        //gs.direction = sf::Vector2i(dirX, dirY);

        inFile.close();
    } 
    else
     {
        cout << "No saved game found. Initializing default state." << std::endl;
        gs.score = 0;
        gs.snakeLength = 1; 
        gs.snakePositionX[0] = maxWidth / 2;  
        gs.snakePositionY[0] = maxHeight / 2;
        gs.foodX = 200;
        gs.foodY = 200;
        gs.direction = sf::Vector2i(0, 1);  // Moving downwards initially
    }
}

void saveState(const GameState &gs) 
{
       ofstream outFile("statepreserve.txt");
       int a=snakestate.getsnakesize();


    if (outFile)
     {
        outFile << score << endl; //score
        outFile << a<<endl; //snake size
                outFile << 1 << endl; //level

        //for (int i = 0; i < gs.snakeLength; i++) 
        //{
          //  outFile << gs.snakePositionX[i] << " " << gs.snakePositionY[i] << "\n";
        //}

        //outFile << gs.foodX << " " << gs.foodY << "\n";
        //outFile << gs.direction.x << " " << gs.direction.y << "\n";

        outFile.close();

    }
}



///////////////////////////////////////////////////////
      
//uses SFML to draw Text on Screen
  void DrawText(sf::RenderWindow& window, const string& text, int x, int y, int size, sf::Color color) {
        sf::Font font;

        if (!font.loadFromFile("Lobster-Regular.ttf"))
         {
            cout << "Handle font loading error";
            return;
        }


        sf::Text t;
        t.setFont(font);
        t.setString(text);
        t.setPosition(x, y);
        t.setCharacterSize(size);
        t.setFillColor(color);
        window.draw(t);
    }




    void run() 
    {
        //GameState currentState;  // Store game state
        //loadState(currentState);
       sf::Clock clock;
       bool isPaused = false;
       bool levelSelected = false;
      
    

      
       


    while (window.isOpen() && !levelSelected)
     {
        sf::Event event;
        while (window.pollEvent(event))
         {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
             {
                if (event.key.code == sf::Keyboard::Enter) 
                {
                    snake.setSpeed(30); 
                    levelSelected = true;
                } 
               
            }
        }

        window.clear(sf::Color::Black);

        DrawText(window, "Snake Game!", 150, 100, 30, sf::Color::Green);
        DrawText(window, "Play", 150, 150, 20, sf::Color::Green);

        window.display();
    }

    ///////////////////////////////////////

    int level = 1;  
    while (window.isOpen())
   {


        sf::Event event;
        while (window.pollEvent(event)) 
        {    
           // loadState(currentState);
            if (event.type == sf::Event::Closed)
                window.close();


             //loadState(currentState);
             //keyboard input
            if (event.type == sf::Event::KeyPressed)
             {
                
                if (event.key.code == sf::Keyboard::Up)
                   { snake.setDirection(sf::Vector2i(0, -1));}
                else if (event.key.code == sf::Keyboard::Down)
                   { snake.setDirection(sf::Vector2i(0, 1));}
                else if (event.key.code == sf::Keyboard::Left)
                    {snake.setDirection(sf::Vector2i(-1, 0));}
                else if (event.key.code == sf::Keyboard::Right)
                   { snake.setDirection(sf::Vector2i(1, 0));}
                   else if (event.key.code == sf::Keyboard::Space)
                   {  saveState(currentState);
                       isPaused = !isPaused;
                   }
                else if (event.key.code == sf::Keyboard::Escape)
                 {
                    saveState(currentState);
                    window.close(); 
                  }

            }
            saveState(currentState);
        }

        if (isPaused)
         {
          
            window.clear();
            DrawText(window, "Game Paused", 150, 250, 30, sf::Color::Red);
            DrawText(window, "Press SPACE to Resume", 150, 300, 20, sf::Color::White);
            window.display();
            continue;
        }

        
        // Moving the snake at a fixed interval
        if (clock.getElapsedTime().asMilliseconds() > ((300-(level*snake.getspeed())))) //adjusted the speed with respect to different levels
        {
            window.clear();

           


   
              snake.move();
              //updates the position of powerfood 
             powerFood.update();
             powerFood.draw(window);  



             if (powerFood.checkCollision(snake.getPosition(), powerFood.getPosition(), 15, 20, 20))//checks the colllision of snake head with powerfood
            {
                powerFood.deactivate();
                snake.grow();
                snake.setSpeed(snake.getspeed() + 1);
                score += 10;
            }

        
            
            snake.makesnake(window);
         
    
           
            if (score >= 20 && score < 60) // level 2
             {
                level = 2;
                snake.setSpeed(80); 
            } 
            else if (score >= 60)
             {
                level = 3;
                snake.setSpeed(80);  //level 3 
            } 
            else
             {
                level = 1;
                snake.setSpeed(90);  //level 1
            }

            if (snake.checkselfcollision()) 
            {
                highscore.addscorepergame(score);
                 plyr.setplayersnakescore(score);
                 highscore.updateHS(score);
                DrawText(window, "Game Over!", 150, 250, 30, sf::Color::Red);
                        window.display();
                        sf::sleep(sf::seconds(2));
                return; 
            }
          



            if (foodcollision())    //if the snake eats food it grows and score increases by 5
            {
                snake.grow();
                score += 5;
                //highscore.updateHS(score);
            
            }

            if (hurdlecollision())   //checks if the snake collided with hurdles: game is over
            {
                highscore.updateHS(score);
            DrawText(window, "Game Over!", 150, 250, 30, sf::Color::Red);
              plyr.setplayersnakescore(score);
                highscore.addscorepergame(score);
                window.display();
                sf::sleep(sf::seconds(2));

                return;
            }

            clock.restart();
    }

        window.clear(sf::Color::Black);


      
        sf::Sprite bgsprite;
        bgsprite.setTexture(bgtexture);
        sf::Vector2u windowSize = window.getSize();
        float width = float(windowSize.x) / bgtexture.getSize().x;
        float height = float(windowSize.y) / bgtexture.getSize().y;
        bgsprite.setScale(width, height);

      



        // Draws the food n screen
        snake.makesnake(window);
        for (int i = 0; i < MAX_FOOD; ++i) 
        {
            foods[i].makefood(window);
        }

        // Draws hurdles on screen
        for (int i = 0; i < MAX_Hurdle; ++i) 
        {
            hurdles[i].makehurdle(window);
        }

        
        //calling DrawText function to write text on screen
        DrawText(window, "Score: " + to_string(score), 10, 10, 20, sf::Color::White);
        DrawText(window, "Level: " + to_string(level), 370, 0, 20, sf::Color::White);
        DrawText(window, "High Score: " + to_string(highscore.gethighscore()), 10, 40, 20, sf::Color::White);

        
        powerFood.draw(window);

        window.display();

    }


}

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////



// ===================== Screen Class =====================
class Screen {
private:
    sf::RenderWindow window;
    sf::Texture splashTexture;
    sf::Sprite splashSprite;

public:
    Screen() {
        if (!splashTexture.loadFromFile("screen.jpg"))
         {
            cout << "Error loading splash image!" << endl;
        }
        splashSprite.setTexture(splashTexture);

        window.create(sf::VideoMode(800, 600), "Splash Screen");
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u imageSize = splashTexture.getSize();
        float x = static_cast<float>(windowSize.x) / imageSize.x;
        float y = static_cast<float>(windowSize.y) / imageSize.y;
        splashSprite.setScale(x, y);
    }

    void displaySplash() 
    {
        sf::Clock clock;
        while (window.isOpen()) 
        {
            sf::Time deltaTime = clock.getElapsedTime();
            if (deltaTime.asSeconds() > 2.0f) 
            {
                return;
            }

            sf::Event event;
            while (window.pollEvent(event))
             {
                if (event.type == sf::Event::Closed)
                 {
                    window.close();
                    return;
                }
            }

            window.clear();
            window.draw(splashSprite);
            window.display();
        }
    }

    sf::RenderWindow& getWindow() {
        return window;
    }
};










class Achievements
{
public:

	string  player_achievements_snake[5];
	string player_achievements_wordle[5];
	string player_achievements_hangman[5];

	static int snake_hs;         //snake game highest score
	static int wordle_hs;        //wordle game highest score   
	static int hangman_hs;       //hangman game highest score
	static string snake_tp;      //snake game top player
	static string wordle_tp;     //wordle game top player
	static string hangman_tp;    //hangman game top player

	Achievements() {}

	void update_achievements(const player& plyr)
	{
		for (int i = 0; i < 5; ++i)
		{
			player_achievements_snake[i] = "";
			player_achievements_wordle[i] = "";
			player_achievements_hangman[i] = "";
		}

		//---set achievemnts for snake game---
			//not using else if so that all if statements are run

		if (plyr.player_snake_score >= 10)
		{
			player_achievements_snake[0] = "First Feast";
		}
		if (plyr.player_snake_score >= 50)
		{
			player_achievements_snake[1] = "Slither Starter";
		}
		if (plyr.player_snake_score >= 100)
		{
			player_achievements_snake[2] = "Tail Chaser";
		}
		if (plyr.player_snake_score >= 200)
		{
			player_achievements_snake[3] = "King Cobra";
		}
		if (plyr.player_snake_score >= 500)
		{
			player_achievements_snake[4] = "Sperpent Supreme";
		}
		//---set achievemnts for wordle game---

		if (plyr.player_wordle_score >= 10)
		{
			player_achievements_wordle[0] = "Letter Learner";
		}
		if (plyr.player_wordle_score >= 50)
		{
			player_achievements_wordle[1] = "Word Weilder";
		}
		if (plyr.player_wordle_score >= 100)
		{
			player_achievements_wordle[2] = "Puzzle Master";
		}
		if (plyr.player_wordle_score >= 200)
		{
			player_achievements_wordle[3] = "Dictionary Dominator";
		}
		if (plyr.player_wordle_score >= 500)
		{
			player_achievements_wordle[4] = "Wordle Titan";
		}
		//---set achievemnts for wordle game---

		if (plyr.player_hangman_score >= 10)
		{
			player_achievements_hangman[0] = "Rope Rookie";
		}
		if (plyr.player_hangman_score >= 50)
		{
			player_achievements_hangman[1] = "Puzzle Solver";
		}
		if (plyr.player_hangman_score >= 100)
		{
			player_achievements_hangman[2] = "Word Warrior";
		}
		if (plyr.player_hangman_score >= 200)
		{
			player_achievements_hangman[3] = "Letter Legend";
		}
		if (plyr.player_hangman_score >= 500)
		{
			player_achievements_hangman[4] = "Hangman Hero";
		}

		//---set snake high score & top player---

		if (plyr.player_snake_score > snake_hs)
		{
			snake_hs = plyr.player_snake_score;
			snake_tp = plyr.name;
		}

		//---set wordle high score & top player---

		if (plyr.player_wordle_score > wordle_hs)
		{
			wordle_hs = plyr.player_wordle_score;
			wordle_tp = plyr.name;
		}

		//---set hangman high score & top player---

		if (plyr.player_hangman_score > hangman_hs)
		{
			hangman_hs = plyr.player_hangman_score;
			hangman_tp = plyr.name;
		}

	}

	//returning coppies so that original arrays are not modified elsewhere
	string* get_snake_achievements()
	{
		return player_achievements_snake;
	}
	string* get_wordle_achievements()
	{
		return player_achievements_wordle;
	}
	string* get_hangman_achievements()
	{
		return player_achievements_hangman;
	}
};

int Achievements::snake_hs = 0;
int Achievements::wordle_hs = 0;
int Achievements::hangman_hs = 0;
string Achievements::snake_tp = "";
string Achievements::wordle_tp = "";
string Achievements::hangman_tp = "";



class Leaderboard {
private:
    static const int MAX_PLAYERS = 10;
    player players[MAX_PLAYERS];
    Achievements achievements[MAX_PLAYERS];
    int playerCount;

    int snakeHighScore = 0;
    int wordleHighScore = 0;
    int hangmanHighScore = 0;

public:
    Leaderboard() : playerCount(0) {
        loadHighScores();
    }

    // Function to load high scores from text files
    void loadHighScores() {
        ifstream snakeFile("SnakeHighScore.txt");
        ifstream wordleFile("wordle_highscore.txt");
        ifstream hangmanFile("hangman_highscore.txt");

        if (snakeFile.is_open()) 
        {
            snakeFile >> snakeHighScore;
            snakeFile.close();
        }

        if (wordleFile.is_open())
         {
            wordleFile >> wordleHighScore;
            wordleFile.close();
        }

        if (hangmanFile.is_open())
         {
            hangmanFile >> hangmanHighScore;
            hangmanFile.close();
        } 
        else
         {
            cout << "Could not open one or more high score files!" << endl;
        }
    }

    // Function to save high scores to text files
    void saveHighScores() {
        ofstream snakeFile("snake_highscore.txt");
        ofstream wordleFile("wordle_highscore.txt");
        ofstream hangmanFile("hangman_highscore.txt");

        if (snakeFile.is_open())
         {
            snakeFile << snakeHighScore;
            snakeFile.close();
        }

        if (wordleFile.is_open())
         {
            wordleFile << wordleHighScore;
            wordleFile.close();
        }

        if (hangmanFile.is_open())
         {
            hangmanFile << hangmanHighScore;
            hangmanFile.close();
        }
         else
          {
            cout << "Cannot save high scores!" << endl;
        }
    }



    void addPlayer(const player& plyr)
     {
        if (playerCount < MAX_PLAYERS) 
        {
            players[playerCount] = plyr;
            achievements[playerCount].update_achievements(plyr);
            playerCount++;
            updateHighScores(plyr);
            saveHighScores();  // Save high scores after each new player
        } 
        else
         {
            cout << "Maximum player limit reached!" << endl;
        }
    }

    void updateHighScores(const player& plyr) 
    {
        if (plyr.player_snake_score > snakeHighScore)
        {
             snakeHighScore = plyr.player_snake_score;
        }
           
        if (plyr.player_wordle_score > wordleHighScore){
            wordleHighScore = plyr.player_wordle_score;
        }
        if (plyr.player_hangman_score > hangmanHighScore){
            hangmanHighScore = plyr.player_hangman_score;
    }
    }



    void displayLeaderboard() const {
        sf::RenderWindow window(sf::VideoMode(800, 600), "Leaderboard");

        sf::Font font;
        if (!font.loadFromFile("Roboto-Regular.ttf")) {
            cout << "Could not load font!" << endl;
            return;
        }

        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("image1.jpeg")) {
            cout << "Could not load background image!" << endl;
            return;
        }
        sf::Sprite background(backgroundTexture);

        while (window.isOpen())
         {
            sf::Event event;
            while (window.pollEvent(event)) 
            {
                if (event.type == sf::Event::Closed)
                 {
                    window.close();
                }
            }

            window.clear();
            window.draw(background);

            sf::Text title("Leaderboard", font, 40);
            title.setStyle(sf::Text::Bold);
            title.setFillColor(sf::Color::Black);
            title.setPosition(300, 20);
            window.draw(title);

            sf::Text leaderboardContent;
            leaderboardContent.setFont(font);
            leaderboardContent.setStyle(sf::Text::Bold);
            leaderboardContent.setCharacterSize(20);
            leaderboardContent.setFillColor(sf::Color::Black);

            string content = "Top Scores:\n\n";
            content += "Snake: " + to_string(snakeHighScore) + "\n";
            content += "Wordle: " + to_string(wordleHighScore) + "\n";
            content += "Hangman: " + to_string(hangmanHighScore) + "\n\n";

            for (int i = 0; i < playerCount; i++) {
                content += "Player " + to_string(i + 1) + " Achievements:\n";
                content += "Snake: ";
                for (int j = 0; j < 5; ++j) {
                    if (achievements[i].player_achievements_snake[j] != "No achievement") {
                        content += achievements[i].player_achievements_snake[j] + ", ";
                    }
                }
                content += "\nWordle: ";
                for (int j = 0; j < 5; ++j) {
                    if (achievements[i].player_achievements_wordle[j] != "No achievement") {
                        content += achievements[i].player_achievements_wordle[j] + ", ";
                    }
                }
                content += "\nHangman: ";
                for (int j = 0; j < 5; ++j) {
                    if (achievements[i].player_achievements_hangman[j] != "No achievement") {
                        content += achievements[i].player_achievements_hangman[j] + ", ";
                    }
                }
                content += "\n\n";
            }

            leaderboardContent.setString(content);
            leaderboardContent.setPosition(50, 100);
            window.draw(leaderboardContent);

            window.display();
        }
    }
};











// ===================== InputSystem Class =====================
class InputSystem {
public:
    static void handleMenuInput(sf::Keyboard::Key key, int& selectedIdx, int optionCount)
     {
        if (key == sf::Keyboard::Up && selectedIdx > 0)
         {
            selectedIdx--;
        } else if (key == sf::Keyboard::Down && selectedIdx < optionCount - 1) 
        {
            selectedIdx++;
        }
    }
};

// ===================== Menu Class =====================
class Menu {
private:
    Screen* screen;
    sf::Font font;
    string* menuOptions;
    sf::Text* txt;
    int selectedIdx;
    int count;
    player plyr;

public:
    Menu(Screen* screenPtr, const string options[], int count): screen(screenPtr), selectedIdx(0), count(count) {
        menuOptions = new string[count];
       txt = new sf::Text[count];

        if (!font.loadFromFile("Lobster-Regular.ttf")) {
            cout<< "Error loading font!" << endl;
        }

        for (int i = 0; i < count; ++i) 
        {
            menuOptions[i] = options[i];
            txt[i].setFont(font);
            txt[i].setString(menuOptions[i]);
             txt[i].setCharacterSize(36);
             txt[i].setFillColor(i == selectedIdx ? sf::Color::Red : sf::Color::White);
             txt[i].setPosition(300, 200 + i * 50);
        }
    }

    ~Menu() {
        delete[] menuOptions;
        delete[]  txt;
    }

    void displayMenu() 
    {
        sf::RenderWindow& window = screen->getWindow();
        while (window.isOpen()) 
        {
            sf::Event event;
            while (window.pollEvent(event)) 
            {
                if (event.type == sf::Event::Closed)
                 {
                    window.close();
                    return;
                }
                if (event.type == sf::Event::KeyPressed)
                 {
                    InputSystem::handleMenuInput(event.key.code, selectedIdx,count);
                    if (event.key.code == sf::Keyboard::Enter)
                     {
                        return;
                    }
                }
            }

            for (int i = 0; i <count; ++i) 
            {
                 txt[i].setFillColor(i == selectedIdx  ?  sf::Color::Red : sf::Color::White);
            }

            window.clear();
            for (int i = 0; i < count; ++i)
             {
                window.draw( txt[i]);
            }
            window.display();
        }
    }

    int getSelectedIndex() const
     {
        return selectedIdx;
    }

};








// ===================== Game Base Class =====================
class Game {
public:
    virtual void start() = 0;                                     // Polymorphic base class
    virtual ~Game() {}



};

// ===================== SnakeGame Class =====================
class startSnakeGame : public Game {
public:
    void start() override {
        cout << "Starting Snake Game..." << endl;

         srand(time(0));
        SnakeGame sgame;
        sgame.run();
    }



};

// ===================== HangmanGame Class =====================
class startHangmanGame : public Game {
public:


    void start() override 
    {
        cout << "Starting Hangman Game..." << endl;
         HangmanGame ge;
          ge.run();
    }


};

// ===================== WordleGame Class =====================
class WordleGame : public Game
 {
public:
    void start() override 
    {
        cout << "Starting Wordle Game..." << endl;
    }
};






// ===================== Game Manager =====================
class GameManager {
private:
    startSnakeGame sG;
    startHangmanGame hg;
    WordleGame wordleGame;
    

public:


    void displayGameOptions(Screen* screen)
     {

        string gameOptions[] = {"Snake Game", "Hangman", "Wordle","Resume SnakeGame", "Back"};
        Menu gameMenu(screen, gameOptions, 5);
        gameMenu.displayMenu();
        SnakeGame load;
        GameState currentState;

        switch (gameMenu.getSelectedIndex())
         {
            case 0:
                sG.start();
                break;
            case 1:
                hg.start();
                break;
            case 2:
                wordleGame.start();
                break;
            case 3:
                 load.loadState(currentState);
                 sG.start();
                //add functinality for resume game
                break;
            case 4:
                cout << "Going Back to Main Menu..." << endl;
                break;
            default:
                break;
        }
    }




};




// ===================== GameBoy Class =====================
class GameBoy {
private:
    Screen* screen;
    //Screen sc;
    Menu menuoptions;
    GameManager gameManager;
     Leaderboard leaderboard; 
     player plyr;

public:
    GameBoy(Screen* screenPtr)
        : screen(screenPtr),
          menuoptions(screenPtr, new string[4]{"Games", "LeaderBoard", "Players", "Exit"}, 4) {}

    void start() {
        if (screen) {
            screen->displaySplash();
            playerdata();
            menuoptions.displayMenu();

            switch (menuoptions.getSelectedIndex())
             {
                case 0:// "Games"
                    gameManager.displayGameOptions(screen);
                    break;
                case 1: // "LeaderBoard"
                    //addTestPlayers(); // Add test players
                    leaderboard.displayLeaderboard();
                    cout << "Displaying LeaderBoard..." << endl;
                    break;
                case 2: // "Players"
                    plyr.setPlayersDetail();
                    cout << "Displaying Players..." << endl;
                    break;
                case 3: // "Exit"
                    cout << "Exiting GameBoy..." << endl;
                    break;
                default:
                    break;
            }
        } 
        else 
        {
            cerr << "Error: Screen not initialized!" << endl;
        }
    }

    void playerdata()
    {
        plyr.setPlayersDetail();

        plyr.storePlayerData();
        leaderboard.addPlayer(plyr);
        leaderboard.updateHighScores(plyr);

      // leaderboard.displayLeaderboard();
       leaderboard.saveHighScores();
    }


};








// ===================== Main Function =====================
int main() {
    Screen screen;
    GameBoy gameBoy(&screen);
    gameBoy.start();
    return 0;
}
