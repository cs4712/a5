#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>


bool isnum(char c);

const int  maxv = 999;


bool isnum(char c){
    return (c >= '0' && c <= '9');
}


int main(int argc, char** argv) {
    int num = -1;
    int *e;
    int **edgs;
    
    // open /dev/urandom
    std::ifstream urandom("/dev/urandom");
    
    // check that it did not fail
    if (urandom.fail())
    {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    
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
                
                //////////////////////////// APPROX-VC-2 //////////////////////////////////
                
                std::vector <int> vertex_cover;
                int random_vertex = 99;  // valid vertex number < 50
                
                
                
                while (true){
                    
                    int flag_edge = 0;
                    
                    
                    //  left edges
                    int total_edges = 0;
                    
                    for (int i=0;i<num;i++){
                        for (int j=0;j<num;j++){
                            if (edgs[i][j] == 1){
                                total_edges++;
                                break;
                            }
                        }
                    }
                    if (total_edges == 0)
                        break;
                    
                    // randomly remove edges
                    unsigned int ran = 1;
                    urandom.read((char*)&ran, sizeof(unsigned int));
                    random_vertex = ran % (num);  //  0 --- v-1
                    
                    int dst = 0;
                    for (dst=0;dst<num;dst++){
                        if (edgs[random_vertex][dst] == 1){
                            for (int j=0;j<num;j++){
                                edgs[random_vertex][j] = maxv;
                                edgs[j][random_vertex] = maxv;
                                edgs[dst][j] = maxv;
                                edgs[j][dst] = maxv;
                            }
                            flag_edge = 1;
                            break;
                        }
                    }
                    
                    if (flag_edge == 0)
                        continue;
                    
                    vertex_cover.push_back(random_vertex);
                    vertex_cover.push_back(dst);
                
                }
                
                if (vertex_cover.size() > 0){
                    std::sort(vertex_cover.begin(), vertex_cover.end());
                    
                    std::cout << "APPROX-VC-2: ";
                    for (int i=0;i<vertex_cover.size()-1;i++){
                        std::cout << vertex_cover[i] << ",";
                    }
                    std::cout << vertex_cover[vertex_cover.size()-1];
                    std::cout << std::endl;
                }
                else{
                    std::cout << "APPROX-VC-2: " << std::endl;
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
    
    // close random stream
    urandom.close();
    
    return 0;
}
