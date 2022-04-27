#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <fstream>
#include <TH1F.h>
//#include <rapidxml.hpp>
#include <sstream>

#include <array>

using namespace std;
//using namespace rapidxml;

char label[500];

vector<float> getData( const string &, string  );
vector<float> getData( const string &text, string tag ){
    std::vector<float> vect;
    string collection;
   //vector<string> collection;
   unsigned int pos = 0, start = -1;
   start = text.find( "<" + tag, pos );
    if (start>0){
        //cout << start << endl;
      start = text.find( ">" , start );
      start++;

      pos = text.find( "</" + tag, start ); 
      collection = text.substr( start, pos - start ) ;
      //cout<<collection<<endl;
      
      std::stringstream ss(collection);
      float i;
      while (ss >> i){
          vect.push_back(i);
          if (ss.peek() == ','){
            ss.ignore();
          }
        }
        //cout<<vect[0]<<endl;
    }
   return vect;
}


std::array<std::array<std::vector<float>,10>, 2> readFile(TString fileName);
std::array<std::array<std::vector<float>,10>, 2> readFile(TString fileName = "./waveform.xml"){
    string file, line, word, temp;      
    vector<string> row;
    std::array< std::array< vector<float>,10>, 2> vx;
    //std::array< std::array< vector<float>,10>, 2> vy;

    int nwave = 0;
    int nrows = 0;
    ifstream myfile (fileName);
    if (myfile.is_open()){
        while ( getline (myfile,line) ){
            if(line.find( "</CHN1>", 0 )!=string::npos) {
                nrows=0;
                nwave++;
            }
            if(nrows>0){
                vector<float> collection = getData(line, "Data");
                vx[0][nwave].push_back(collection[0]);
                vx[1][nwave].push_back(collection[1]);
                //h_waves.Fill(collection[0], collection[1]);
                nrows++;
            }
            if(line.find( "<CHN1>", 0 )!=string::npos) nrows++;
        }
    }
    return vx;
}

void readXMLFiles(){
    TH1F *h_waves[10];
    TH1F *h_counts[10];
    for(int w=0; w<10; w++){
        sprintf(label,"h_waves_%d",w);
        h_waves[w] = new TH1F(label, label, 150, 0, 1500);
        sprintf(label,"h_counts_%d",w);
        h_counts[w] = new TH1F(label, label, 150, 0, 1500);
        //h_waves.push_back(h_waves_tmp);
    }
    std::array<std::array<std::vector<float>,10>, 2> vxy = readFile("./waveform.xml");

    for(int w=0; w<10; w++){
        for(int i=0; i<vxy[0][w].size(); i++){
            h_waves[w]->Fill( vxy[0][w][i] , vxy[1][w][i] );
            h_counts[w]->Fill( vxy[0][w][i]  );
        }
        h_waves[w]->Divide(h_counts[w]);
    }

    
    for(int w=0; w<10; w++){
        if(w==0) h_waves[w]->Draw("HIST");
        else h_waves[w]->Draw("HISTsame");
    }

}