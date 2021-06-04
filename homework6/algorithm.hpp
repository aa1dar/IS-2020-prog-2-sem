#ifndef C___ALGORITHM_HPP
#define C___ALGORITHM_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <sstream>
using namespace std;


typedef vector<int> vi;



int applicableMoves[] = { 0, 262143, 259263, 74943, 74898 };



int affectedCubies[][8] = {
        {  0,  1,  2,  3,  0,  1,  2,  3 },   
        {  4,  7,  6,  5,  4,  5,  6,  7 },   
        {  0,  9,  4,  8,  0,  3,  5,  4 },   
        {  2, 10,  6, 11,  2,  1,  7,  6 },   
        {  3, 11,  7,  9,  3,  2,  6,  5 },   
        {  1,  8,  5, 10,  1,  0,  4,  7 },   
};

vi applyMove ( int move, vi state ) {
    int turns = move % 3 + 1;
    int face = move / 3;
    while( turns-- ){
        vi oldState = state;
        for( int i=0; i<8; i++ ){
            int isCorner = i > 3;
            int target = affectedCubies[face][i] + isCorner*12;
            int killer = affectedCubies[face][(i&3)==3 ? i-3 : i+1] + isCorner*12;;
            int orientationDelta = (i<4) ? (face>1 && face<4) : (face<2) ? 0 : 2 - (i&1);
            state[target] = oldState[killer];
            
            state[target+20] = oldState[killer+20] + orientationDelta;
            if( !turns )
                state[target+20] %= 2 + isCorner;
        }
    }
    return state;
}

int inverse ( int move ) {
    return move + 2 - 2 * (move % 3);
}



int phase;



vi id ( vi state ) {

    
    if( phase < 2 )
        return vi( state.begin() + 20, state.begin() + 32 );

    
    if( phase < 3 ){
        vi result( state.begin() + 31, state.begin() + 40 );
        for( int e=0; e<12; e++ )
            result[0] |= (state[e] / 8) << e;
        return result;
    }

    
    if( phase < 4 ){
        vi result( 3 );
        for( int e=0; e<12; e++ )
            result[0] |= ((state[e] > 7) ? 2 : (state[e] & 1)) << (2*e);
        for( int c=0; c<8; c++ )
            result[1] |= ((state[c+12]-12) & 5) << (3*c);
        for( int i=12; i<20; i++ )
            for( int j=i+1; j<20; j++ )
                result[2] ^= state[i] > state[j];
        return result;
    }

    
    return state;
}



stringstream findAnswer(stringstream& is ) {

    stringstream os;
   
    string goal[] = { "UF", "UR", "UB", "UL", "DF", "DR", "DB", "DL", "FR", "FL", "BR", "BL",
                      "UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR" };

    
    vi currentState( 40 ), goalState( 40 );
    for( int i=0; i<20; i++ ){

        
        goalState[i] = i;

        
        string cubie;
        is>>cubie;
        while( (currentState[i] = find( goal, goal+20, cubie ) - goal) == 20){
            cubie = cubie.substr( 1 ) + cubie[0];
            currentState[i+20]++;
        }
    }

    
    while( ++phase < 5 ){

       
        vi currentId = id( currentState ), goalId = id( goalState );
        if( currentId == goalId )
            continue;

        
        queue<vi> q;
        q.push( currentState );
        q.push( goalState );

        
        map<vi,vi> predecessor;
        map<vi,int> direction, lastMove;
        direction[ currentId ] = 1;
        direction[ goalId ] = 2;

        
        while( 1 ){

           
            vi oldState = q.front();
            q.pop();
            vi oldId = id( oldState );
            int& oldDir = direction[oldId];

            
            for( int move=0; move<18; move++ ){
                if( applicableMoves[phase] & (1 << move) ){

                    //--- Apply the move.
                    vi newState = applyMove( move, oldState );
                    vi newId = id( newState );
                    int& newDir = direction[newId];

                    
                    
                    if( newDir  &&  newDir != oldDir ){

                        
                        if( oldDir > 1 ){
                            swap( newId, oldId );
                            move = inverse( move );
                        }

                        
                        vi algorithm( 1, move );
                        while( oldId != currentId ){
                            algorithm.insert( algorithm.begin(), lastMove[ oldId ] );
                            oldId = predecessor[ oldId ];
                        }
                        while( newId != goalId ){
                            algorithm.push_back( inverse( lastMove[ newId ] ));
                            newId = predecessor[ newId ];
                        }

                        //--- Print 
                        for( int i=0; i<(int)algorithm.size(); i++ ){
                            os << "UDFBLR"[algorithm[i]/3] << algorithm[i]%3+1<<" ";
                            currentState = applyMove( algorithm[i], currentState );
                        }

                        
                        goto nextPhasePlease;
                    }

                    
                    if( ! newDir ){
                        q.push( newState );
                        newDir = oldDir;
                        lastMove[ newId ] = move;
                        predecessor[ newId ] = oldId;
                    }
                }
            }
        }
        nextPhasePlease:
        ;
    }
    return os;
}


#endif //C___ALGORITHM_HPP
