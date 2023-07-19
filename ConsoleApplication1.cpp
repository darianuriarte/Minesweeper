#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream> ifstream
#include <unordered_map>
#include <vector>
#include <sstream>
using namespace std;
#include <random>
#include <ctime>


class Random
{
    mt19937 random;
public:
    int Int(int min, int max) {
        {
            std::uniform_int_distribution<int> dist(min, max);
            return dist(random);
        }
    }
};

class Space {
    sf::Vector2f position;
public:
    void SetPositionX(int x) {
        position.x = x;
    }
    void SetPositionY(int y) {
        position.y = y;
    }
    sf::Vector2f getPosition() {
        return position;
    }

};

class TextureManager  {
    //Map to store textures
    unordered_map<string, sf::Texture> textures;
public:
    void loadFromFile(string name){
        string path = "images/";
        path += name + ".png";
        textures[name].loadFromFile(path);
    }
    sf::Texture& GetTexture(string name) {
        if (textures.find(name) == textures.end()) {
            loadFromFile(name);
        }
        return TextureManager::textures[name];
    }

}; 

class Tile : public Space{
private:
    bool hasMine = false;
    bool hasFlag = false;
    bool revealed = false;
    //Store AdjacentTiles
    vector<Tile*> adjacentTiles;
    float numberOfMines = 0; //Display Number of Adjancent Tiles;
    //Section where Tile is located
    bool isCorner = false;
    bool isLine = false;
    bool isNormal = false;
    //Maximum Number of possible Neighborhoods
    int maxNeighborhoods = 0;
    vector<Tile*> neighborTiles; //Store AdjacentTiles that aren't flagged or revealed
  
public:
    Tile() {
        revealed = false;
    }
    bool hasMineCheck(){
        return hasMine;
    }
    bool hasFlagStatus() {
        return hasFlag;
    }
    void reveal() {
        revealed = true;
    }
    void unreveal() {
        revealed = false;
    }
    void flagTile() {
        hasFlag = true;
    }
    void unflagTile() {
        hasFlag = false;
    }
    void setMine (int number){
        if (number == 1)
            hasMine = true;
        else
            hasMine = false;
    }
    void SetAdjacentTiles(Tile* newTile) {
        adjacentTiles.push_back(newTile);
    }
    vector<Tile*> GetAdjacentTiles() {
        return adjacentTiles;
    }

    void setneighborTiles() {
        neighborTiles = adjacentTiles;
        while (neighborTiles.size() != 8) {
            neighborTiles.push_back(nullptr);
        }   
    }
    void Clear() {
        adjacentTiles.clear();
        neighborTiles.clear();
        
        
    }
    int GetNumberOfMines() {
        return numberOfMines;
    }
    void SetNumberOfMines() {
        numberOfMines = (maxNeighborhoods - adjacentTiles.size());       
    }

    void setNormal(bool set) {
        isNormal = set;
    }

    bool getNormal() {
        return isNormal;
    }
    void setCorner(bool set) {
        isCorner = set;
    }
    bool getCorner() {
        return isCorner;
    }
    void setLine(bool set) {
        isLine = set;
    }
    bool getLine() {
        return isLine;
    }
    void setMaxNeighborhoods() {
        if (isCorner)
            maxNeighborhoods = 3;
        if (isLine)
            maxNeighborhoods = 5;
        if (isNormal)
            maxNeighborhoods = 8;
    }
    int getMaxNeighborhood() {
        return maxNeighborhoods;
    }

    bool flagCheck() {
        return hasFlag;
    }

    bool isRevealed(){
        return revealed;
    }

    int checkAdjacents() {
        int count = 0;
        for (int i = 0; i < 8; i++) {
            if (neighborTiles[i] != nullptr) {
                if (neighborTiles[i]->hasMineCheck())
                    count++;
            }
        }
        return count;
    }

    vector <Tile*> GetNeighborTiles(){
        return neighborTiles;
    }

    void revealAdjacents() {
       
        if (this != nullptr && this->checkAdjacents() == 0 && !this->hasMineCheck() && !this->flagCheck()) {
            for (int i = 0; i < 8; i++) {
                if (this->neighborTiles[i] != nullptr) {
             
                    if (!this->neighborTiles[i]->isRevealed() && !this->neighborTiles[i]->hasMineCheck() && this->flagCheck() == false) {
                        this->neighborTiles[i]->reveal();
                     
                        
                        if (this->neighborTiles[i]->numberOfMines == 0) {
                            this->neighborTiles[i]->revealAdjacents();
                        }
                    }
                }
            }

        }
    }


};

class Face : public Space {
    bool normalStatus = false; //Normal Status
    bool win = false; //Win Game
    bool GameOver = false; //Game Over
    bool reset = false; //Reset Game
    
public:
    Face() {
        normalStatus = true;
        sf::Vector2f position;
    }

    //Set Game Over
    void declareGameOver(){
        normalStatus = false;
        GameOver = true;
        win = false;
    }

    bool isGameOver(){
        return GameOver;
    }
    //Set Normal Status
    void setNormalGametoNormal () {
        normalStatus = true;
        GameOver = false;
        win = false;
        reset = false;
    }
    bool getNormalStatus() {
        return normalStatus;
    }
    //Set Game Win
    void setWin() {
        win = true;
        normalStatus = false;
        GameOver = false;
    }
    bool getWinSatus() {
        return win;
    }
    //Reset Game
    void resetGame() {
        reset = true;
    }
    bool getReset() {
        return reset;
    }
  
};

vector<vector<int> > LoadBoard(string boardName, int columns, int rows) {
    vector <int> Temp;
    vector<vector<int>> rowsInt;
    vector<string> rowsString;
    
    for (int i = 0; i < rows; i++) {
        rowsInt.push_back(Temp);
    }

    ifstream myFile;
    myFile.open(boardName, std::ifstream::in);

    //Extract Data as String
    if (myFile.is_open()) {
        string value;
        for (int i = 0; i < rows; i++) {
            myFile >> value;
            rowsString.push_back(value);   
        }
    }

    for (int i = 0; i < rows; i++) {
        vector<char> v(rowsString[i].begin(), rowsString[i].end());
        for (int j = 0; j < columns; j++) {
            rowsInt[i].push_back(v[j] - '0');
        }
     }
    return rowsInt;
}

vector<vector<Tile>> ObtainAdjacentTiles(vector<vector<Tile>> Tiles, int numberOfRows, int numberOfColumns) {
    
    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            //Top Left Corner
            if (i == 0 && j == 0) {
                if (Tiles[i][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j + 1]);
                if (Tiles[i + 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j]);
                if (Tiles[i + 1][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j + 1]);
            }
            //Top Right Corner
            if (i == 0 && j == numberOfColumns - 1) {
                if (Tiles[i][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j - 1]);
                if (Tiles[i + 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j]);
                if (Tiles[i + 1][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j - 1]);
            }
            //Bottom Left Corner
            if (i == numberOfRows - 1 && j == false) {
                if (Tiles[i][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j + 1]);
                if (Tiles[i - 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j]);
                if (Tiles[i - 1][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j + 1]);
            }
            //Bottom Right Corner
            if (i == numberOfRows - 1 && j == numberOfColumns - 1) {
                if (Tiles[i][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j - 1]);
                if (Tiles[i - 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j]);
                if (Tiles[i - 1][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j - 1]);
            }
            //Top Line
            if (i == 0 && (j > 0 && j < numberOfColumns - 1)) {
                if (Tiles[i][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j - 1]);
                if (Tiles[i][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j + 1]);
                if (Tiles[i + 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j]);
                if (Tiles[i + 1][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j - 1]);
                if (Tiles[i + 1][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j + 1]);
            }
            //Bottom Line
            if (i == numberOfRows - 1 && (j > 0 && j < numberOfColumns - 1)) {
                if (Tiles[i][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j - 1]);
                if (Tiles[i][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j + 1]);
                if (Tiles[i - 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j]);
                if (Tiles[i - 1][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j - 1]);
                if (Tiles[i - 1][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j + 1]);
            }
            //Left Line
            if (i > 0 && (i < numberOfRows - 1) && j == false) {
                if (Tiles[i][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j + 1]);
                if (Tiles[i + 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j]);
                if (Tiles[i + 1][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j + 1]);
                if (Tiles[i - 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j]);
                if (Tiles[i - 1][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j + 1]);
            }
            //Right Line
            if (i > 0 && (i < numberOfRows - 1) && j == numberOfColumns -1) {
                if (Tiles[i-1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i-1][j]);
                if (Tiles[i - 1][j-1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j-1]);
                if (Tiles[i][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j - 1]);
                if (Tiles[i + 1][j -1 ].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j-1]);
                if (Tiles[i + 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j]);
            }
           //All other Locations with 8 neighborhoods 
            if (i > 0 && i < (numberOfRows - 1) && (j > 0 && j < (numberOfColumns - 1))) {

                if (Tiles[i-1][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i-1][j - 1]);
                if (Tiles[i - 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j]);
                if (Tiles[i - 1][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i - 1][j + 1]);
                if (Tiles[i][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j -1]);
                if (Tiles[i][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i][j + 1]);
                if (Tiles[i + 1][j - 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j - 1]);
                if (Tiles[i + 1][j].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i + 1][j]);
                if (Tiles[i+1][j + 1].hasMineCheck() == false)
                    Tiles[i][j].SetAdjacentTiles(&Tiles[i+1][j + 1]);
            }
        }
    }
    return Tiles;
}

int main()
{
    //Variables
    int numberOfColumns = 0;
    int width = 0;
    int heigth = 0;
    int numberOfRows = 0;
    int numberOfMines = 0;
    ifstream file;
    bool debugAction = false;

    //Handling config.cfg file
    file.open("boards/config.cfg");
    file >> numberOfColumns;
    file >> numberOfRows;
    file >> numberOfMines;
    file.close();
    int originalNumberofMines = numberOfMines;


    //Create Sprites (Images)
    TextureManager textureHandler;
    sf::Sprite debugSprite(textureHandler.GetTexture("debug"));
    sf::Sprite digitsSprite1(textureHandler.GetTexture("digits"));
    sf::Sprite digitsSprite2(textureHandler.GetTexture("digits"));
    sf::Sprite digitsSprite3(textureHandler.GetTexture("digits"));
    sf::Sprite digitsSprite4(textureHandler.GetTexture("digits"));
    sf::Sprite face_happySprite(textureHandler.GetTexture("face_happy"));
    sf::Sprite face_loseSprite(textureHandler.GetTexture("face_lose"));
    sf::Sprite face_winSprite(textureHandler.GetTexture("face_win"));
    sf::Sprite flagSprite(textureHandler.GetTexture("flag"));
    sf::Sprite mineSprite(textureHandler.GetTexture("mine"));
    sf::Sprite number_1Sprite(textureHandler.GetTexture("number_1"));
    sf::Sprite number_2Sprite(textureHandler.GetTexture("number_2"));
    sf::Sprite number_3Sprite(textureHandler.GetTexture("number_3"));
    sf::Sprite number_4Sprite(textureHandler.GetTexture("number_4"));
    sf::Sprite number_5Sprite(textureHandler.GetTexture("number_5"));
    sf::Sprite number_6Sprite(textureHandler.GetTexture("number_6"));
    sf::Sprite number_7Sprite(textureHandler.GetTexture("number_7"));
    sf::Sprite number_8Sprite(textureHandler.GetTexture("number_8"));
    sf::Sprite test_1Sprite(textureHandler.GetTexture("test_1"));
    sf::Sprite test_2Sprite(textureHandler.GetTexture("test_2"));
    sf::Sprite test_3Sprite(textureHandler.GetTexture("test_3"));
    sf::Sprite tile_hiddenSprite(textureHandler.GetTexture("tile_hidden"));
    sf::Sprite tile_revealedSprite(textureHandler.GetTexture("tile_revealed"));

    //Create Tiles
    vector<vector<Tile>> Tiles;

    //Initializing Vector to store Tiles
    for (int i = 0; i < numberOfRows; i++) {
        //Vector to store column elements
        vector<Tile> v1;
        for (int j = 0; j < numberOfColumns; j++) {
            v1.push_back(Tile()); 
        }
        //Pushing back above 1D vector to create 2D Vector
        Tiles.push_back(v1);
        for (int j = 0; j < numberOfColumns; j++) {
        }
        
    }

    //Generate Random Mines
    Random randomNumbers;
    int countMines = 0; //Ensure Mines stay under assigned number
    
    for (int i = 0; i < numberOfMines; i++) {
        Tiles[randomNumbers.Int(0, numberOfRows - 1)][randomNumbers.Int(0, numberOfColumns - 1)].setMine(1);
    }
   
    //Calcualte Tiles Position Type // //Calculate Max Number of Possible Adjancent Tiles
    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            //Corner Tiles
            if (((i == 0 && j == 0) || (i == numberOfRows - 1 && j == 0) || (i == numberOfRows - 1 && j == numberOfColumns - 1) ||
                (i == 0 && j == numberOfColumns - 1)))
                Tiles[i][j].setCorner(true);
            //Top, Lef, Bottom, Right Lines
            else if ((i == 0 && (j > 0 && j < numberOfColumns - 1) || (i > 0 && (i < numberOfRows - 1) && j == 0)
                || (i > 0 && (i < numberOfRows - 1) && j == numberOfColumns - 1) ||
                (i == numberOfRows - 1 && (j > 0 && j < numberOfColumns - 1))))
                Tiles[i][j].setLine(true);
            //All other Locations
            else if ((i > 0 && i < (numberOfRows - 1) && (j > 0 && j < (numberOfColumns - 1))))
                Tiles[i][j].setNormal(true);
            Tiles[i][j].setMaxNeighborhoods();
        }
    }

    //Set Windows Size to Project Specified Format
    width = numberOfColumns * 32;
    heigth = (numberOfRows * 32) + 100;
    //Start Window
    sf::RenderWindow window(sf::VideoMode(width, heigth), "Minesweeper");
    sf::Style::Default;
    //Set Windows Size to Project Specified Format
    width = numberOfColumns * 32;
    heigth = (numberOfRows * 32) + 100;
 

    //Initialize Tiles and Setting position //
    int start1 = 0;
    int start2 = 0;

    for (int i = 0; i < numberOfRows; i++) {
        start2 = 0;
        for (int j = 0; j < numberOfColumns; j++) {
            Tiles[i][j].SetPositionX(start2);
            Tiles[i][j].SetPositionY(start1);
            start2 = start2 + 32;
        }
        start1 = start1 + 32;
    }

    //Initializing Face and Setting Position
    Face face;
    if((width / 2) - 32 > width - 320) {
        face.SetPositionX(width - 320);
        cout << "fue menor" << endl;
    }
    else {
        face.SetPositionX((width / 2) - 32);
    }

    face.SetPositionY(heigth - 100);

    //Obtaining Adjacent Tiles that doesn't contain Mines
    Tiles = ObtainAdjacentTiles(Tiles, numberOfRows, numberOfColumns);

    for (int i = 0; i < numberOfRows; i++) {
        for (int j = 0; j < numberOfColumns; j++) {
            //Set Number of Mines
            Tiles[i][j].SetNumberOfMines();
        }
    }

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {

            //Clear Window. Set Background color to white
            const sf::Color& color = sf::Color(255, 255, 255, 255);
            window.clear(color);

            //Setting Location and Displaying Face
            if (face.getNormalStatus() == true) {
                face_happySprite.setPosition(face.getPosition());
                window.draw(face_happySprite);
            }

            //Setting Location and Displaying Toggles
            Space debug;
            Space test_1;
            Space test_2;
            Space test_3;
            test_3.SetPositionY(heigth - 100);
            test_3.SetPositionX(width - 64);
            test_3Sprite.setPosition(test_3.getPosition());
            window.draw(test_3Sprite);
            test_2.SetPositionY(heigth - 100);
            test_2.SetPositionX(width - 128);
            test_2Sprite.setPosition(test_2.getPosition());
            window.draw(test_2Sprite);
            test_1.SetPositionY(heigth - 100);
            test_1.SetPositionX(width - 192);
            test_1Sprite.setPosition(test_1.getPosition());
            window.draw(test_1Sprite);
            debug.SetPositionY(heigth - 100);
            debug.SetPositionX(width - 256);
            debugSprite.setPosition(debug.getPosition());
            window.draw(debugSprite);

            //Check If Game Over
            if (face.isGameOver()) {
                //Change Face to Lose
                face_loseSprite.setPosition(face.getPosition());
                window.draw(face_loseSprite);
                //Dispplay the rest of the Mines
                debugAction = true;
            }

            //Check if Win
            if (face.getWinSatus() == true) {
                //Change Face to Lose
                face_winSprite.setPosition(face.getPosition());
                window.draw(face_winSprite);
            }

            //Display Tiles
            for (int i = 0; i < numberOfRows; i++) {
                for (int j = 0; j < numberOfColumns; j++) {
                    
                    //Check if Debug Mode is One
                    if (Tiles[i][j].hasMineCheck() && debugAction == true && face.getWinSatus() == false) {
                        mineSprite.setPosition(Tiles[i][j].getPosition());
                        window.draw(mineSprite);
                    }
                    //Check if there are flags
                    else if (Tiles[i][j].flagCheck()) {
                        flagSprite.setPosition(Tiles[i][j].getPosition());
                        window.draw(flagSprite);
                    }
                    //If Tile is hidden draw hidden sprite
                    else if (Tiles[i][j].isRevealed() == false)
                    {
                        tile_hiddenSprite.setPosition(Tiles[i][j].getPosition());
                        window.draw(tile_hiddenSprite);
                    }
                    //IF tile is Revealed
                    else if (Tiles[i][j].isRevealed())
                    {
                        switch (Tiles[i][j].GetNumberOfMines()) {
                        case 1:
                            number_1Sprite.setPosition(Tiles[i][j].getPosition());
                            window.draw(number_1Sprite);
                            break;
                        case 2:
                            number_2Sprite.setPosition(Tiles[i][j].getPosition());
                            window.draw(number_2Sprite);
                            break;
                        case 3:
                            number_3Sprite.setPosition(Tiles[i][j].getPosition());
                            window.draw(number_3Sprite);
                            break;
                        case 4:
                            number_4Sprite.setPosition(Tiles[i][j].getPosition());
                            window.draw(number_4Sprite);
                            break;
                        case 5:
                            number_5Sprite.setPosition(Tiles[i][j].getPosition());
                            window.draw(number_5Sprite);
                            break;
                        case 6:
                            number_6Sprite.setPosition(Tiles[i][j].getPosition());
                            window.draw(number_6Sprite);
                            break;
                        case 7:
                            number_7Sprite.setPosition(Tiles[i][j].getPosition());
                            window.draw(number_7Sprite);
                            break;
                        case 8:
                            number_8Sprite.setPosition(Tiles[i][j].getPosition());
                            window.draw(number_8Sprite);
                            break;
                        default:
                            tile_revealedSprite.setPosition(Tiles[i][j].getPosition());
                            window.draw(tile_revealedSprite);
                        }    
                    }
                }
            }
      
                //Displaying Number of Mines
                Space digits;
                //Extract Digits into Vectos
                vector<int> digitValues;
                int number = numberOfMines;
                if (number < 0)
                    number = number * -1;
                while (number > 0) {
                    int digit = number % 10;
                    number /= 10;
                    digitValues.push_back(digit);
                }

                //If number is negative print negative sign
                if (numberOfMines < 0) {
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(0);
                    digitsSprite1.setPosition(digits.getPosition());
                    digitsSprite1.setTextureRect(sf::IntRect(210, 0, 21, 32));
                    window.draw(digitsSprite1);
                }

                //Print Remaining Digits
                //Check number of digits in Mines Values
                if (numberOfMines < 0)
                    number = numberOfMines * -1;
                else
                    number = numberOfMines;
                
                //If only one digit display the two other digits in front as 0
                if (number < 10 && number != 0) {
                    int offset = 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite2.setPosition(digits.getPosition());
                    digitsSprite2.setTextureRect(sf::IntRect(0, 0, 21, 32));
                    window.draw(digitsSprite2);
                    offset = offset + 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite3.setPosition(digits.getPosition());
                    digitsSprite3.setTextureRect(sf::IntRect(0, 0, 21, 32));
                    window.draw(digitsSprite3);
                    //Display Actual Number of Mines in Sprite 4
                    offset = offset + 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite4.setPosition(digits.getPosition());
                    digitsSprite4.setTextureRect(sf::IntRect((21 * digitValues[digitValues.size() - 1]), 0, 21, 32));
                    window.draw(digitsSprite4);
                }
                if (number >= 10 && number < 100) {
                    int offset = 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite2.setPosition(digits.getPosition());
                    digitsSprite2.setTextureRect(sf::IntRect(0, 0, 21, 32));
                    window.draw(digitsSprite2);
                    offset = offset + 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite3.setPosition(digits.getPosition());
                    digitsSprite3.setTextureRect(sf::IntRect((21 * digitValues[digitValues.size() - 1]), 0, 21, 32));
                    window.draw(digitsSprite3);
                    //Display Actual Number of Mines in Sprite 4
                    offset = offset + 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite4.setPosition(digits.getPosition());
                    digitsSprite4.setTextureRect(sf::IntRect((21 * digitValues[digitValues.size() - 2]), 0, 21, 32));
                    window.draw(digitsSprite4);
                }
                if (number > 100) {
                    int offset = 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite2.setPosition(digits.getPosition());
                    digitsSprite2.setTextureRect(sf::IntRect((21 * digitValues[digitValues.size() - 1]), 0, 21, 32));
                    window.draw(digitsSprite2);
                    offset = offset + 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite3.setPosition(digits.getPosition());
                    digitsSprite3.setTextureRect(sf::IntRect((21 * digitValues[digitValues.size() - 2]), 0, 21, 32));
                    window.draw(digitsSprite3);
                    //Display Actual Number of Mines in Sprite 4
                    offset = offset + 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite4.setPosition(digits.getPosition());
                    digitsSprite4.setTextureRect(sf::IntRect((21 * digitValues[digitValues.size() - 3]), 0, 21, 32));
                    window.draw(digitsSprite4);
                }
                //If  Number is equal to 0
                if (numberOfMines == 0) {
                    int offset = 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite2.setPosition(digits.getPosition());
                    digitsSprite2.setTextureRect(sf::IntRect(0, 0, 21, 32));
                    window.draw(digitsSprite2);
                    offset = offset + 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite3.setPosition(digits.getPosition());
                    digitsSprite3.setTextureRect(sf::IntRect(0, 0, 21, 32));
                    window.draw(digitsSprite3);
                    //Display Actual Number of Mines in Sprite 4
                    offset = offset + 21;
                    digits.SetPositionY(heigth - 100);
                    digits.SetPositionX(offset);
                    digitsSprite4.setPosition(digits.getPosition());
                    digitsSprite4.setTextureRect(sf::IntRect(0, 0, 21, 32));
                    window.draw(digitsSprite4);
                }
                // ************************ When Right Click on Tile ***********************************
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && face.isGameOver() == false)
                {
                    // get the local mouse position (relative to a window)
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    int Xcoordinate = 0;
                    int Ycoordinate = 0;
                    int end = 32;
                    for (int i = 0; i < width; i = i + 32) {
                        if (localPosition.x > i && localPosition.x < end) {
                            Xcoordinate = i;
                        }
                        end = end + 32;
                    }
                    end = 32;
                    for (int i = 0; i < heigth; i = i + 32) {
                        if (localPosition.y > i && localPosition.y < end) {
                            Ycoordinate = i;
                        }
                        end = end + 32;
                    }
                    for (int i = 0; i < numberOfRows; i++) {
                        for (int j = 0; j < numberOfColumns; j++) {
                            
                            if (Tiles[i][j].getPosition().x == Xcoordinate && Tiles[i][j].getPosition().y == Ycoordinate && Tiles[i][j].isRevealed() == false
                                && Tiles[i][j].flagCheck() == false) {
                                Tiles[i][j].flagTile() ;
                                numberOfMines = numberOfMines --;
                                break;
                            }
                            
                            if (Tiles[i][j].getPosition().x == Xcoordinate && Tiles[i][j].getPosition().y == Ycoordinate && Tiles[i][j].isRevealed() == false) {
                                
                                Tiles[i][j].unflagTile() ;
                                
                                numberOfMines = numberOfMines++;
                                break;

                            }

                            if (Tiles[i][j].isRevealed() == true && Tiles[i][j].flagCheck() == true) {
                                Tiles[i][j].unreveal();
                            }
                        }
                    }
                }
                
                //************************** Reset Game (Click On Face)**************************************
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    
                {
                    // get the local mouse position (relative to a window)
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    if (localPosition.x > face_happySprite.getPosition().x
                        && localPosition.x < (face_happySprite.getPosition().x + 64) &&
                        localPosition.y > face_happySprite.getPosition().y
                        && localPosition.y < (face_happySprite.getPosition().y + 64)) {

                        face.setNormalGametoNormal();
                        numberOfMines = originalNumberofMines;
                        
                        for (int i = 0; i < numberOfRows; i++) {
                            for (int j = 0; j < numberOfColumns; j++) {
                                Tiles[i][j].setMine(0);
                                Tiles[i][j].Clear();
                                Tiles[i][j].unreveal();
                                Tiles[i][j].unflagTile();
                            }
                        }

                        //Generate Random Mines
                        countMines = 0; //Ensure Mines stay under assigned number
                        for (int i = 0; i < numberOfMines; i++) {
                            Tiles[randomNumbers.Int(0, numberOfRows - 1)][randomNumbers.Int(0, numberOfColumns - 1)].setMine(1);
                        }
                        //Update Adjacent Tiles;
                        Tiles = ObtainAdjacentTiles(Tiles, numberOfRows, numberOfColumns);
                        for (int i = 0; i < numberOfRows; i++) {
                            for (int j = 0; j < numberOfColumns; j++) {
                                //Set Number of Mines
                                Tiles[i][j].SetNumberOfMines();
                            }
                        }  
                    }
                }

                //*******************Load Test 1**************************************

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    // get the local mouse position (relative to a window)
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    if (localPosition.x > test_1Sprite.getPosition().x
                        && localPosition.x < (test_1Sprite.getPosition().x + 64) &&
                        localPosition.y > test_1Sprite.getPosition().y
                        && localPosition.y < (test_1Sprite.getPosition().y + 64)) {

                        //If Left Mouse Click Load Test Board
                        //Reset Board and Number of Mines
                        numberOfMines = 0;
                        vector<vector<int> > minesLocation = LoadBoard("boards/testboard1.brd", numberOfColumns, numberOfRows);
                        for (int i = 0; i < numberOfRows; i++) {
                            for (int j = 0; j < numberOfColumns; j++) {
                                Tiles[i][j].setMine(minesLocation[i][j]);
                                //Reset Adjacent Tiles
                                Tiles[i][j].Clear();
                                //Hide Tiles
                                Tiles[i][j].unreveal();
                                Tiles[i][j].unflagTile();
                                if (Tiles[i][j].hasMineCheck() == true)
                                    numberOfMines++;
                            }
                        }
                        //Update Adjacent Tiles;
                        Tiles = ObtainAdjacentTiles(Tiles, numberOfRows, numberOfColumns);
                        for (int i = 0; i < numberOfRows; i++) {
                            for (int j = 0; j < numberOfColumns; j++) {
                                //Set Number of Mines
                                Tiles[i][j].SetNumberOfMines(); 
                            }
                        }
                     }
                }
                //*******************Load Test 2**************************************
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) )
                {
                    // get the local mouse position (relative to a window)
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    if (localPosition.x > test_2Sprite.getPosition().x
                        && localPosition.x < (test_2Sprite.getPosition().x + 64) &&
                        localPosition.y > test_2Sprite.getPosition().y
                        && localPosition.y < (test_2Sprite.getPosition().y + 64)) {

                        //If Left Mouse Click Load Test Board
                        //Reset Board and Number of Mines
                        numberOfMines = 0;
                        vector<vector<int> > minesLocation = LoadBoard("boards/testboard2.brd", numberOfColumns, numberOfRows);
                        for (int i = 0; i < numberOfRows; i++) {
                            for (int j = 0; j < numberOfColumns; j++) {
                                Tiles[i][j].setMine(minesLocation[i][j]);
                                //Reset Adjacent Tiles
                                Tiles[i][j].Clear();
                                //Hide Tiles
                                Tiles[i][j].unreveal();
                                Tiles[i][j].unflagTile();
                                if (Tiles[i][j].hasMineCheck() == true)
                                    numberOfMines++;
                            }
                        }
                        //Update Adjacent Tiles;
                        Tiles = ObtainAdjacentTiles(Tiles, numberOfRows, numberOfColumns);
                        for (int i = 0; i < numberOfRows; i++) {
                            for (int j = 0; j < numberOfColumns; j++) {
                                //Set Number of Mines
                                Tiles[i][j].SetNumberOfMines();
                            }
                        }
                    }
                }
                //*******************Load Test 3**************************************

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    // get the local mouse position (relative to a window)
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    if (localPosition.x > test_3Sprite.getPosition().x
                        && localPosition.x < (test_3Sprite.getPosition().x + 64) &&
                        localPosition.y > test_3Sprite.getPosition().y
                        && localPosition.y < (test_3Sprite.getPosition().y + 64)) {

                        //If Left Mouse Click Load Test Board
                        //Reset Board and Number of Mines
                        numberOfMines = 0;
                        vector<vector<int> > minesLocation = LoadBoard("boards/testboard3.brd", numberOfColumns, numberOfRows);
                        for (int i = 0; i < numberOfRows; i++) {
                            for (int j = 0; j < numberOfColumns; j++) {
                                Tiles[i][j].setMine(minesLocation[i][j]);
                                //Reset Adjacent Tiles
                                Tiles[i][j].Clear();
                                //Hide Tiles
                                Tiles[i][j].unreveal();
                                Tiles[i][j].unflagTile();
                                if (Tiles[i][j].hasMineCheck() == true)
                                    numberOfMines++;
                            }
                        }
                        //Update Adjacent Tiles;
                        Tiles = ObtainAdjacentTiles(Tiles, numberOfRows, numberOfColumns);
                        for (int i = 0; i < numberOfRows; i++) {
                            for (int j = 0; j < numberOfColumns; j++) {
                                //Set Number of Mines
                                Tiles[i][j].SetNumberOfMines();
                            }
                        }
                    }
                }

                //***************************** Debug ************************************
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && face.isGameOver() == false)
                {
                    // get the local mouse position (relative to a window)
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    if (debugAction == false && localPosition.x > debugSprite.getPosition().x
                        && localPosition.x < (debugSprite.getPosition().x + 64) &&
                        localPosition.y > debugSprite.getPosition().y
                        && localPosition.y < (debugSprite.getPosition().y + 64)) {
                        
                        //If Left Mouse Click Show Mines
                        for (int i = 0; i < numberOfRows; i++) {
                            for (int j = 0; j < numberOfColumns; j++) {
                                if (Tiles[i][j].hasMineCheck()) {
                                    debugAction = true;
                                }
                            }
                        }
                    }
                    //If Debug is already activated turn it off
                        else if (debugAction == true && localPosition.x > debugSprite.getPosition().x
                        && localPosition.x < (debugSprite.getPosition().x + 64) &&
                        localPosition.y > debugSprite.getPosition().y
                        && localPosition.y < (debugSprite.getPosition().y + 64))
                        debugAction = false;
                }             
                // ************************ WHEN LEFT CLICK ON TILE ***********************************
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    // get the local mouse position (relative to a window)
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    int Xcoordinate = 0;
                    int Ycoordinate = 0;
                    int end = 32;
                    for (int i = 0; i < width; i = i + 32) {
                        if (localPosition.x > i && localPosition.x < end) {
                            Xcoordinate = i;
                        }
                        end = end + 32;
                    }
                    end = 32;
                    for (int i = 0; i < heigth; i = i + 32) {
                        if (localPosition.y > i && localPosition.y < end) {
                            Ycoordinate = i;
                        }
                        end = end + 32;
                    }
                    for (int i = 0; i < numberOfRows; i++) {
                        for (int j = 0; j < numberOfColumns; j++) {
                            //If there is a Mine
                            if (Tiles[i][j].getPosition().x == Xcoordinate && Tiles[i][j].getPosition().y == Ycoordinate && Tiles[i][j].flagCheck() == false 
                                &&Tiles[i][j].hasMineCheck()) {
                                face.declareGameOver();                          
                                break;
                            }
                            //If there is no mine
                            if (Tiles[i][j].getPosition().x == Xcoordinate && Tiles[i][j].getPosition().y == Ycoordinate && Tiles[i][j].flagCheck() == false
                                && Tiles[i][j].hasMineCheck() == false && Tiles[i][j].isRevealed() == false) {
     
                                if (Tiles[i][j].GetNumberOfMines() == 0) {
                                    for (int m = 0; m < numberOfRows; m++) {
                                        for (int n = 0; n < numberOfColumns; n++) {
                                            Tiles[m][n].setneighborTiles();
                                        }
                                    }
                                    Tiles[i][j].revealAdjacents();
                                    Tiles[i][j].reveal();
                                }
                                else {
                                    Tiles[i][j].reveal();
                                }
                                break;
                            }
                          
                        }
                    }
                }

                /******************************* Check if Win************************************************/
                int numberReveals = 0;
                for (int i = 0; i < numberOfRows; i++) {
                    for (int j = 0; j < numberOfColumns; j++) {
                        if (Tiles[i][j].isRevealed() == true) {
                            numberReveals++;
                        }
                    }
                }

                if (numberReveals == ((numberOfRows*numberOfColumns) - numberOfMines)) {
                    for (int i = 0; i < numberOfRows; i++) {
                        for (int j = 0; j < numberOfColumns; j++) {
                            if (Tiles[i][j].hasMineCheck())
                                Tiles[i][j].flagTile();   
                        }
                    }
                    face.setWin();
                    numberOfMines = 0;
                }


                window.display();

                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }

      return 0;
    
}
