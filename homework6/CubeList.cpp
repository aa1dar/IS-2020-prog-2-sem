#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <random>
#include <fstream>
#include "algorithm.hpp"

using namespace std;

enum Color{
    W = 0,
    Y = 1,
    G = 2,
    B = 3,
    O = 4,
    R = 5
};

enum Command{
    Up,
    Down,
    Right,
    Left,
    Front,
    Back,

};







class Sticker {

private:
    Color color_;
public:

    Sticker();

    Sticker(Color &col){
        this->color_ = col;
    };

    void setCol(const Color &col){
        this->color_ = col;
    }

    const Color& getCol() const{
        return this->color_;
    }


};

class Face{
private:

    vector<Sticker*> cells;
    Sticker* cub11;
    Sticker* cub12;
    Sticker* cub13;
    Sticker* cub21;
    Sticker* cub22;
    Sticker* cub23;
    Sticker* cub31;
    Sticker* cub32;
    Sticker* cub33;
    Color centerCol_;

public:


    Face(Color col):
            cub11(new Sticker(col)),
            cub12(new Sticker(col)),
            cub13(new Sticker(col)),
            cub21(new Sticker(col)),
            cub22(new Sticker(col)),
            cub23(new Sticker(col)),
            cub31(new Sticker(col)),
            cub32(new Sticker(col)),
            cub33(new Sticker(col))
    {
        cells.push_back(cub11);
        cells.push_back(cub12);
        cells.push_back(cub13);
        cells.push_back(cub21);
        cells.push_back(cub22);
        cells.push_back(cub23);
        cells.push_back(cub31);
        cells.push_back(cub32);
        cells.push_back(cub33);


        centerCol_ = col;
    };

    void paint(const Color &col){
        for(auto i: cells){
            i->setCol(col);
        }
    }

    bool isCenterCorrect() const {
        return cub22->getCol() == centerCol_;
    }

    bool isSolved() const {
        std::vector<bool> isTrue;

        std::transform(cells.begin(), cells.end(),
                       std::back_inserter(isTrue),[this](Sticker * c){return c->getCol() == centerCol_;});
        bool solved = true;

        for(auto i:isTrue){
            solved *= i;
        }
        return solved;
    }

    vector<Sticker*> & getCell(){
        return cells;

    }


    vector<Sticker> swith(vector<Sticker> s ){
        Sticker tmp = s[0];
        s[0] = s[2];
        s[2] = tmp;
        return s;
    }


    void rotateCw(){
        vector<Sticker> coll1 = getColumn(1);

        setColumn(1, getRow(3));
        setRow(3, swith(getColumn(3)));
        setColumn(3, getRow(1));
        setRow(1, swith(coll1));
    }

    void rotateCCw(){
        vector<Sticker> coll1 = getColumn(1);

        setColumn(1, swith(getRow(1)));
        setRow(1, getColumn(3));
        setColumn(3, swith(getRow(3)));
        setRow(3, coll1);
    }
    vector<Sticker> getColumn(int i){
        switch (i){
            case 1:
                return vector<Sticker>{*cub11, *cub21,*cub31};
            case 2:
                return vector<Sticker>{*cub12, *cub22,*cub32};
            case 3:
                return vector<Sticker>{*cub13, *cub23,*cub33};
            default:
                return vector<Sticker>{};
        }
    }

    void  setColumn(int i, const vector<Sticker> &column){
        switch(i){
            case 1:
                cub11->setCol(column.at(0).getCol());
                cub21->setCol(column.at(1).getCol());
                cub31->setCol(column.at(2).getCol());
                break;
            case 2:
                cub12->setCol(column.at(0).getCol());
                cub22->setCol(column.at(1).getCol());
                cub32->setCol(column.at(2).getCol());
                break;
            case 3:
                cub13->setCol(column.at(0).getCol());
                cub23->setCol(column.at(1).getCol());
                cub33->setCol(column.at(2).getCol());
                break;
            default:
                return;
        }
    }

    const vector<Sticker> getRow(int i){
        switch (i){
            case 1:
                return vector<Sticker>{*cub11, *cub12,*cub13};
            case 2:
                return vector<Sticker>{*cub21, *cub22,*cub23};
            case 3:
                return vector<Sticker>{*cub31, *cub32,*cub33};

            default:
                return vector<Sticker>{};
        }
    }

    void  setRow(int i, const vector<Sticker> &column){
        switch(i){
            case 1:
                cub11->setCol(column.at(0).getCol());
                cub12->setCol(column.at(1).getCol());
                cub13->setCol(column.at(2).getCol());
                break;
            case 2:
                cub21->setCol(column.at(0).getCol());
                cub22->setCol(column.at(1).getCol());
                cub23->setCol(column.at(2).getCol());
                break;
            case 3:
                cub31->setCol(column.at(0).getCol());
                cub32->setCol(column.at(1).getCol());
                cub33->setCol(column.at(2).getCol());
                break;
            default:
                return;
        }
    }
};



class Cube{
public:

private:
    Face* front;
    Face* back;
    Face* left;
    Face* right;
    Face* up;
    Face* down;
    map<string, void(Cube::*)(int)> translateCom;

    map<char,string> translateAlgorithm;

    vector<Face*> mainCube;

    vector<string> wayToAns;
    vector<string> command;


public:

    Cube():front (new Face(Color::W)),
           back    (new Face(Color::Y)),
           left    (new Face(Color::G)),
           right   (new Face(Color::B)),
           up    (new Face(Color::O)),
           down  (new Face(Color::R)) {

        mainCube.push_back(front);
        mainCube.push_back(back);
        mainCube.push_back(left);
        mainCube.push_back(right);
        mainCube.push_back(up);
        mainCube.push_back(down);

        command = vector<string>{"U1","U2","U3","D1","D2","D3","F1","F2","F3","L1","L2","L3","R1","R2","R3"};

        translateCom["U1"] = &Cube::upCW;
        translateCom["U2"] = &Cube::upCW;
        translateCom["U3"] = &Cube::upCCW;

        translateCom["D1"] = &Cube::downCW;
        translateCom["D2"] = &Cube::downCW;
        translateCom["D3"] = &Cube::downCCW;

        translateCom["F1"] = &Cube::frontCW;
        translateCom["F2"] = &Cube::frontCW;
        translateCom["F3"] = &Cube::frontCCW;

        translateCom["B1"] = &Cube::backCW;
        translateCom["B2"] = &Cube::backCW;
        translateCom["B3"] = &Cube::backCCW;

        translateCom["L1"] = &Cube::leftCW;
        translateCom["L2"] = &Cube::leftCW;
        translateCom["L3"] = &Cube::leftCCW;

        translateCom["R1"] = &Cube::rightCW;
        translateCom["R2"] = &Cube::rightCW;
        translateCom["R3"] = &Cube::rightCCW;


        translateAlgorithm['4'] = 'U';
        translateAlgorithm['1'] = 'B';
        translateAlgorithm['0'] = 'F';
        translateAlgorithm['5'] = 'D';
        translateAlgorithm['2'] = 'L';
        translateAlgorithm['3'] = 'R';




    };

    void transformCube(const string& command){
        int val = 1;
        if(command[1]=='2')
            val = 2;
        (this->*translateCom[command])(val);

    }

    bool isValid() const{
        bool center = true;
        vector<bool> corrCenter;

        //transform(mainCube.begin(),mainCube.end(),true,std::back_inserter(corrCenter),
        //[](const Face *f){return f->isCenterCorrect();});
        for (auto&& i: mainCube){
            if(i->isCenterCorrect())
                corrCenter.push_back(true);
            else
                corrCenter.push_back(false);
        }


        for(auto i:corrCenter)
            center *= i;





        vector<int> vecOfCountOfCol(7);

        for(auto&& i: mainCube){
            for(auto&& j: i->getCell()){
                vecOfCountOfCol[(*j).getCol()]+=1;
            }
        };


        int countOfCol = count(vecOfCountOfCol.begin()+1,vecOfCountOfCol.end(),9);

        return center&&(countOfCol == 6);


    }

    bool isSolved() const{
        vector<bool> vecOfSolvedFaces;

        transform(mainCube.begin(), mainCube.end(),
                  std::back_inserter(vecOfSolvedFaces),
                  [](Face * f){return f->isSolved();});

        bool ans = true;
        for(auto i:vecOfSolvedFaces)
            ans *= i;

        return ans;

    }

    void Scramble(int steps){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 12);



        for(int i=0;i<steps;i++)
            transformCube(command[dis(gen)-1]);

    }

    vector<Sticker> swith(vector<Sticker> s ){
        Sticker tmp = s[0];
        s[0] = s[2];
        s[2] = tmp;
        return s;
    }


    void frontCW(int i = 1){
        for(int k = 0;k<i;k++) {
            front->rotateCw();

            vector<Sticker> oldVec = up->getRow(3);
            up->setRow(3, swith(left->getColumn(3)));
            left->setColumn(3, down->getRow(1));
            down->setRow(1, swith(right->getColumn(1)));
            right->setColumn(1, oldVec);
        }
    };

    void frontCCW(int i = 0) {
        front->rotateCCw();

        vector<Sticker> oldVec = up->getRow(3);
        up->setRow(3, right->getColumn(1));
        right->setColumn(1, swith(down->getRow(1)));
        down->setRow(1, left->getColumn(3));
        left->setColumn(3, swith(oldVec));
    }

    void backCW(int i = 1) {
        for(int k = 0;k<i;k++) {
            back->rotateCw();

            vector<Sticker> oldVec = up->getRow(1);
            up->setRow(1, right->getColumn(3));
            right->setColumn(3, swith(down->getRow(3)));
            down->setRow(3, left->getColumn(1));
            left->setColumn(1, swith(oldVec));
        }
    }

    void backCCW(int i = 0) {
        back->rotateCCw();

        vector<Sticker> oldVec = up->getRow(1);
        up->setRow(1, swith(left->getColumn(1)));
        left->setColumn(1, down->getRow(3));
        down->setRow(3, swith(right->getColumn(3)));
        right->setColumn(3, oldVec);
    }

    void leftCW(int i = 1) {
        for(int k = 0;k<i;k++) {
            left->rotateCw();

            vector<Sticker> oldVec = up->getColumn(1);
            up->setColumn(1, swith(back->getColumn(3)));
            back->setColumn(3, swith(down->getColumn(1)));
            down->setColumn(1, front->getColumn(1));
            front->setColumn(1, oldVec);
        }
    }

    void leftCCW(int i = 0) {
        left->rotateCCw();

        vector<Sticker> oldVec = up->getColumn(1);
        up->setColumn(1, front->getColumn(1));
        front->setColumn(1, down->getColumn(1));
        down->setColumn(1, swith(back->getColumn(3)));
        back->setColumn(3, swith(oldVec));
    }

    void rightCW(int i = 1) {
        for(int k = 0;k<i;k++) {
            right->rotateCw();

            vector<Sticker> oldVec = up->getColumn(3);
            up->setColumn(3, front->getColumn(3));
            front->setColumn(3, down->getColumn(3));
            down->setColumn(3, swith(back->getColumn(1)));
            back->setColumn(1, swith(oldVec));
        }
    }

    void rightCCW(int i = 0) {
        right->rotateCCw();

        vector<Sticker> oldVec = up->getColumn(3);
        up->setColumn(3, swith(back->getColumn(1)));
        back->setColumn(1, swith(down->getColumn(3)));
        down->setColumn(3, front->getColumn(3));
        front->setColumn(3, oldVec);
    }

    void  upCW(int i = 1) {
        for(int k = 0;k<i;k++) {
            up->rotateCw();

            vector<Sticker> oldVec = front->getRow(1);
            front->setRow(1, right->getRow(1));
            right->setRow(1, back->getRow(1));
            back->setRow(1, left->getRow(1));
            left->setRow(1, oldVec);
        }
    }

    void  upCCW(int i = 0) {
        up->rotateCCw();

        vector<Sticker> oldVec = front->getRow(1);
        front->setRow(1, left->getRow(1));
        left->setRow(1, back->getRow(1));
        back->setRow(1, right->getRow(1));
        right->setRow(1, oldVec);
    }



    void downCW(int i = 1) {
        for(int k = 0;k<i;k++) {
            down->rotateCw();

            vector<Sticker> oldVec = front->getRow(3);
            front->setRow(3, left->getRow(3));
            left->setRow(3, back->getRow(3));
            back->setRow(3, right->getRow(3));
            right->setRow(3, oldVec);
        }
    }

    void downCCW(int i = 0) {
        down->rotateCCw();

        vector<Sticker> oldVec = front->getRow(3);
        front->setRow(3, right->getRow(3));
        right->setRow(3, back->getRow(3));
        back->setRow(3, left->getRow(3));
        left->setRow(3, oldVec);
    }


    stringstream stateOfCube(){
        stringstream ss;
        vector<string> way;



        way.push_back(to_string(up->getColumn(2)[2].getCol()) +to_string(front->getColumn(2)[0].getCol()));

        way.push_back(to_string(up->getRow(2)[2].getCol()) +to_string(right->getColumn(2)[0].getCol()));

        way.push_back(to_string(up->getColumn(2)[0].getCol()) +to_string(back->getColumn(2)[0].getCol()));

        way.push_back(to_string(up->getRow(2)[0].getCol()) +to_string(left->getColumn(2)[0].getCol()));

        way.push_back(to_string(down->getColumn(2)[0].getCol()) +to_string(front->getColumn(2)[2].getCol()));

        way.push_back(to_string(down->getRow(2)[2].getCol()) +to_string(right->getColumn(2)[2].getCol()));

        way.push_back(to_string(down->getColumn(2)[2].getCol()) +to_string(back->getColumn(2)[2].getCol()));

        way.push_back(to_string(down->getRow(2)[0].getCol()) +to_string(left->getColumn(2)[2].getCol()));

        way.push_back(to_string(front->getRow(2)[2].getCol()) +to_string(right->getRow(2)[0].getCol()));
        way.push_back(to_string(front->getRow(2)[0].getCol()) +to_string(left->getRow(2)[2].getCol()));

        way.push_back(to_string(back->getRow(2)[0].getCol()) +to_string(right->getRow(2)[2].getCol()));
        way.push_back(to_string(back->getRow(2)[2].getCol()) +to_string(left->getRow(2)[0].getCol()));

        way.push_back(to_string(up->getColumn(3)[2].getCol()) +to_string(front->getColumn(3)[0].getCol())+to_string(right->getColumn(1)[0].getCol()) ); //ufr

        way.push_back(to_string(up->getColumn(3)[0].getCol()) +to_string(right->getColumn(3)[0].getCol()) +to_string(back->getColumn(1)[0].getCol()) ); //urb

        way.push_back(to_string(up->getColumn(1)[0].getCol()) +to_string(back->getColumn(3)[0].getCol())+to_string(left->getColumn(1)[0].getCol()) ); //ubl

        way.push_back(to_string(up->getColumn(1)[2].getCol()) +to_string(left->getColumn(3)[0].getCol())+to_string(front->getColumn(1)[0].getCol()) ); //ulf

        way.push_back(to_string(down->getColumn(3)[0].getCol()) +to_string(right->getColumn(1)[2].getCol())+to_string(front->getColumn(3)[2].getCol()) ); //drf

        way.push_back(to_string(down->getColumn(1)[0].getCol()) +to_string(front->getColumn(1)[2].getCol())+to_string(left->getColumn(3)[2].getCol()) ); //dfl

        way.push_back(to_string(down->getColumn(1)[2].getCol()) +to_string(left->getColumn(1)[2].getCol())+to_string(back->getColumn(3)[2].getCol()) ); //dlb

        way.push_back(to_string(down->getColumn(3)[2].getCol()) +to_string(back->getColumn(1)[2].getCol())+to_string(right->getColumn(3)[2].getCol()) ); //dbr

        for(const string& t: way){
            for(char i: t){
                ss<<translateAlgorithm[i];

            }
            ss<<" ";

        };


        return ss;
    }


    void toSolve(){
        stringstream is = stateOfCube();

        stringstream os = findAnswer(is);

        string buffer;

        while(os>>buffer){
            wayToAns.push_back(buffer);
            transformCube(buffer);
            cout<<buffer<<" ";
        }



    }



    const vector<Color> getState() const {



        vector<Color> state;
        for (auto&& f : mainCube)
        {
            for (auto&& c : f->getCell())
            {
                state.push_back((*c).getCol());
            }
        }

        return state;


    }

     void setState(vector<Color> &state)  {
        auto it = state.begin();

        for (auto&& f : mainCube)
        {
            for (auto&& c : f->getCell())
            {
                (*c).setCol(*(it++));
            }
        }




    }

    void writeFile(){

        ofstream fout("state.txt");
        vector<Color> state = getState();
        vector<string> side = {"F","B","L","R","U","D"};
        int k=0;
        int l = 0;
        auto t = side.begin();
        fout<<(*t)<<endl;
        for(auto i:state){
            k+=1;
            fout<<i<<' ';

            if(k==3){
                fout<<endl;
                k=0;
                l+=1;
            }
            if(l==3 ) {
                l=0;
                fout << endl;
                if((++t)!=side.end())
                    fout << (*t) << endl;
            }

        }
        fout.close();

    }

    void writeCons(){


        vector<Color> state = getState();
        vector<string> side = {"F","B","L","R","U","D"};
        int k=0;
        int l = 0;
        auto t = side.begin();
        cout<<(*t)<<endl;
        for(auto i:state){
            k+=1;
            cout<<i<<' ';

            if(k==3){
                cout<<endl;
                k=0;
                l+=1;
            }
            if(l==3 ) {
                l=0;
                cout << endl;
                if((++t)!=side.end())
                    cout << (*t) << endl;
            }

        }


    }

    void readFile(){
        ifstream fin("state.txt");
        vector<Color> state;

        fin.seekg(0);
        int k=0;
        string buf;
        fin>>buf;
        int col;
        for(int i = 0;i<54;i++){
            k+=1;

            fin>>col;
            state.push_back(Color(col));

            if(k==3){
                k=0;
            }
            if((i+1)%9 == 0 ) {
                fin >> buf;
            }

        }
        setState(state);

        fin.close();
    }


};


int main(){


    

    Cube c;
    string q;




    //vector<string> ff = {"F3","U1", "F3", "D3", "L3", "D3", "F3", "U3", "L2", "D3"};


    //for(const string& q:ff)
    //    c.transformCube(q);


    c.Scramble(5);
    c.writeCons();


    c.toSolve();

    c.writeFile();







    return 0;
}
