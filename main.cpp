#include <stdio.h>
#include <iostream>
#include <vector>
#include<algorithm>

bool isnum(char c);

const int  maxv = 999;


bool isnum(char c){
    return (c >= '0' && c <= '9');
}


int main(int argc, char** argv) {
    int num = -1;
    int *e;
    int **edgs;
    
    while(!std::cin.eof()){
        char comma;
        int err = 0;
        static int err1 = 0;
        static int err2 = 0;
        std::cin.clear();
        std::cin.sync();
        std::cin.get(comma);
        
        switch(comma){
            case 'V' :{
                
                // read the amount of vertices
                if (err2==4){
                    std::cerr << "Error : repeative V command" << std::endl;
                    break;
                }
                
                std::cin >> num;
                
                if (num<0){
                    std::cerr << "Error : no vertex input" << std::endl;
                    break;
                }
                
                // create empty matrix
                
                edgs = new int *[num];
                for (int i=0;i<num;i++){
                    edgs[i] = new int[num];
                }
                
                for (int i=0;i<num;i++){
                    for (int j=0;j<num;j++){
                        edgs[i][j] = maxv;
                    }
                }
                
                err1 = 1;       //successful V command
                err2 = 4;       //first successful V comand
                break;
            }
                
                
                
            case 'E' : {
                
                std::string line;
                std::cin >> line;
                int k = line.size();
                e = new int[k];
                int result;
                int n = 0, m = 0;
                
                if (err1==3){
                    std::cerr << "Error : repeative E command" << std::endl;
                    break;
                }
                
                if (err1!=1){
                    std::cerr << "Error : no V command input" << std::endl;
                    break;
                }
                
                while (n<k){
                    result = 0;
                    
                    if (isnum(line[n])){
                        
                        if (line[n-1]=='-'){
                            err = 2;
                            break;
                        }
                        result = line[n] - '0';
                        
                        while (n<k && isnum(line[++n])){
                            result = result * 10 + line[n] - '0';
                        }
                        
                        if (result >= num || result < 0){
                            err = 1;
                            break;
                        }
                        
                        else
                            e[m++] = result;
                    }
                    ++n;
                }
                
                if (err==1){
                    std::cerr << "Error: vertex doesn't exist" << std::endl;
                    err = 0;
                    break;
                }
                
                if (err==2){
                    std::cerr << "Error : negative number" << std::endl;
                    err = 0;
                    break;
                }
                
                // memory edges
                if (m%2){
                    std::cerr << "Error : invalid edges" << std::endl;
                    break;
                }
                
                for (int j=0;j<m;j=j+2){
                    
                    if (e[j] == e[j+1])
                        continue;
                    edgs[e[j]][e[j+1]] = 1;
                    edgs[e[j+1]][e[j]] = 1;
                }
                err1 = 3;       //first successful E command
                err2 = 0;       //can input V again after successful E command
                
                //////////////////////////// APPROX-VC-1 //////////////////////////////////
                
                std::vector <int> vertex_cover;
             
                while (true){
                    int degree_max = 0;
                    int max_degree_vertex = 99;     //valid vertex number < 50
                    
                    for (int i=0;i<num;i++){
                        int temp = 0;
                        for (int j=0;j<num;j++){
                            if (edgs[i][j]==1 && i!=j){
                                temp++;
                            }
                        }
                        
                        if (temp > degree_max){
                            degree_max = temp;
                            max_degree_vertex = i;
                        }
                    }
                    
                    if (degree_max == 0)
                        break;  // all edges have been removed
                    
                    for (int i=0;i<num;i++){
                        for (int j=0;j<num;j++){
                            if ((max_degree_vertex == i) || (max_degree_vertex == j)){
                                edgs[i][j] = maxv;
                            }
                        }
                    }
                    
                    vertex_cover.push_back(max_degree_vertex);
                    
                }
                
                if (vertex_cover.size() > 0){
                    std::sort(vertex_cover.begin(), vertex_cover.end());
                    
                    std::cout << "APPROX-VC-1: ";
                    for (int i=0;i<vertex_cover.size()-1;i++){
                        std::cout << vertex_cover[i] << ",";
                    }
                    std::cout << vertex_cover[vertex_cover.size()-1];
                    std::cout << std::endl;
                }
                else{
                    std::cout << "APPROX-VC-1: " << std::endl;
                }

            /////////////////////////////////////////////////////////////////////////////
                
            }
                
                // other command
            default :
                
                break;
                
        }
        
    }
    
    // delete matrix
    
    delete[] e;
    
    for (int i=0;i<num;i++)
        delete[] edgs[i];
    delete[] edgs;
    
    return 0;
}
