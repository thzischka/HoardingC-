//
// Created by Andrew Eeckman on 2/7/18.
//


#include "Player.h"
#include <iostream>
#include <fstream>
#include <string>


#ifndef HOARDINGCPP_SPACE_H
#define HOARDINGCPP_SPACE_H

using std::string;

namespace Monopoly {

    class Board;

    class boardSpace {
    public:
        boardSpace();

        /***** PROPERTY SPECIFIC (EXCLUDE NAME AND TYPE) *****/

        virtual void setType(const string &typeIn);
        virtual void setSetID(int set);
        virtual void setIntraID(int intra);
        virtual void setName(const string &nameIn);
        virtual void setPropertyCost(int cost);
        virtual void setHouseCost(int hCost);
        virtual void setHotelCost(int htCost);
        virtual void setStartRent(int sR);
        virtual void setRent(int r);
        virtual void setRentHouse(int hRent);
        virtual void setRentHotel(int htRent);
        virtual void setOwned(bool o);
        virtual void setOwnedBy(int by);
        virtual void setPositionOnBoard(int pos);
        virtual void setNumHouses(int numH);
        virtual void setNumHotels(int numHot);

        virtual string getType();
        virtual int getSetID();
        virtual int getIntraID();
        virtual string getName();
        virtual int getPropertyCost();
        virtual int getHouseCost();
        virtual int getHotelCost();
        virtual int getStartRent();
        virtual int getRent();
        virtual int getRentHouse();
        virtual int getRentHotel();
        virtual bool getOwned();
        virtual int getOwnedBy();
        virtual int getPositionOnBoard();
        virtual int getNumHouses();
        virtual int getNumHotels();
        virtual bool getCanHouse();

        virtual void readInProperty(ifstream& readIn) {}
        virtual void readInGo(ifstream& readIn) {}
        virtual void readInFreeParking(ifstream& readIn) {}
        virtual void readInPay(ifstream& readIn) {}

        /***** GO SPECIFIC *****/
        virtual void setMoneyAroundGo(int money) {}
        virtual int getMoneyAroundGo() { return -99; };

        virtual void setSalMultiAroundGo(int salMulti) {}
        virtual int getSalMultiAroundGo() { return -99; }

        virtual void activate(Player& activatingPlayer) = 0;
        virtual void display() = 0;
    };

    class propertyType : public boardSpace {
    public:
        propertyType(Board &board);

        void setType(const string &typeIn) override {
            type = typeIn;
        }
        void setSetID(int set) override {
            setID = set;
        }
        void setIntraID(int intra) override {
            intraID = intra;
        }
        void setName(const string &nameIn) override {
            name = nameIn;
        }
        void setPropertyCost(int cost) override {
            propertyCost = cost;
        }
        void setHouseCost(int hCost) override {
            houseCost = hCost;
        }
        void setHotelCost(int htCost) override {
            hotelCost = htCost;
        }
        void setStartRent(int sR) override {
            rent = sR;
        }
        void setRent(int r) override {
            rent = r;
        }
        void setRentHouse(int hRent) override {
            rentWithHouse = hRent;
        }
        void setRentHotel(int htRent) override {
            rentWithHotel = htRent;
        }
        void setOwned(bool o) override {
            owned = o;
        }
        void setOwnedBy(int o) override {
            ownedBy = o;
        }
        void setPositionOnBoard(int pos) override {
            positionOnBoard = pos;
        }
        void setNumHouses(int numH) override {
            numHouses = numH;
        }
        void setNumHotels(int numHot) override {
            numHotels = numHot;
        }

        string getType() override {
            return type;
        }
        int getSetID() override {
            return setID;
        }
        int getIntraID() override {
            return intraID;
        }
        string getName() override {
            return name;
        }
        int getPropertyCost() override {
            return propertyCost;
        }
        int getHouseCost() override {
            return houseCost;
        }
        int getHotelCost() override {
            return hotelCost;
        }
        int getStartRent() override {
            return startRent;
        }
        int getRent() override {
            return rent;
        }
        int getRentHouse() override {
            return rentWithHouse;
        }
        int getRentHotel() override {
            return rentWithHotel;
        }
        bool getOwned() override {
            return owned;
        }
        int getOwnedBy() override {
            return ownedBy;
        }
        int getPositionOnBoard() override {
            return positionOnBoard;
        }
        int getNumHouses() override {
            return numHouses;
        }
        int getNumHotels() override {
            return numHotels;
        }
        bool getCanHouse() override {
            return canHouse;
        }

        Board& board;

        void activate(Player& activatingPlayer) override;
        void display() override;

        void readInProperty(ifstream& readIn) override;

    private:
        string type;
        int setID;
        int intraID;
        string name;
        int propertyCost;
        int houseCost;
        int hotelCost;
        int startRent;
        int rent;
        int rentWithHouse;
        int rentWithHotel;

        bool owned;
        int ownedBy;
        int numHouses;
        int numHotels;
        bool canHouse;
        bool canHotel;
        bool canSellHouse;
        bool canSellHotel;

        int positionOnBoard;
    };

    class goType : public boardSpace {
    public:
        goType(Board &board);

        void setType(const string& t) override {
            type = t;
        }
        void setMoneyAroundGo(int money) override {
            moneyAroundGo = money;
        }
        void setSalMultiAroundGo(int salMulti) override {
            salMultiAroundGo = salMulti;
        }
        void setName(const string& n) override {
            name = n;
        }
        void setPositionOnBoard(int pos) override {
            positionOnBoard = pos;
        }

        string getType() override {
            return type;
        }
        int getMoneyAroundGo() override {
            return moneyAroundGo;
        }
        int getSalMultiAroundGo() override {
            return salMultiAroundGo;
        }
        string getName() override {
            return name;
        }
        int getPositionOnBoard() override {
            return positionOnBoard;
        }

        void readInGo(ifstream& readIn) override;

        Board& board;

        void activate(Player& activatingPlayer) override;
        void display() override;

    private:
        string type;
        int moneyAroundGo;
        int salMultiAroundGo;
        string name;
        int positionOnBoard;
    };

    class FreeParking : public boardSpace {
    public:
        FreeParking(Board &board);
        void activate(Player& activatingPlayer) override;
        void display() override;
        void readInFreeParking(ifstream& readIn) override;
        void setPositionOnBoard(int pos) override {
            positionOnBoard = pos;
        }
        int getPositionOnBoard() override {
            return positionOnBoard;
        }

        Board& board;

    private:
        string type;
        string name;
        int positionOnBoard;
    };

    class Pay : public boardSpace {
    public:
        Pay(Board& board);
        void activate(Player& activatingPlayer) override;
        void display() override;
        void readInPay(ifstream& readIn) override;
        void setPositionOnBoard(int pos) override {
            positionOnBoard = pos;
        }
        int getPositionOnBoard() override {
            return positionOnBoard;
        }

        Board& board;

    private:
        string type;
        string name;
        int moneyToBePaid;
        int positionOnBoard;
    };
}


#endif //HOARDINGCPP_SPACE_H
