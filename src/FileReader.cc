#include "Randomize.hh"
#include "FileReader.hh"

FileReader *FileReader::Instance_1 = nullptr;
std::vector<double> FileReader::cEn;
std::vector<double> FileReader::cIn;
FileReader::FileReader()
{
  
}

FileReader::~FileReader()
{
}

FileReader *FileReader::Instance()
{
  if (!Instance_1)
  {
    Instance_1 = new FileReader();
  }
  return Instance_1;
}

void FileReader::GetAnEvent()
{
  // std::ifstream inputFile ("6HeDecay5.txt");
  // std::ifstream inputFile ("19NeDecay.txt");
  // std::ifstream inputFile ("Mono1MeV.txt");
  // std::ifstream inputFile ("133BaGammas.txt");
  // std::ifstream inputFile ("137CsGammas.txt");
  // std::ifstream inputFile ("60CoGammas.txt");
  std::ifstream inputFile("90Sr90YDecaycdf.txt");
  // std::ifstream inputFile ("90SrDecaypdf.txt");
  // std::ifstream inputFile ("90YDecaypdf.txt");

  std::cout << "HI CLINT" << std::endl;
  return;


  while (!inputFile.eof())
  {
    double a, b;
    // i++;
    // G4cout << i <<" "<< a<<"\n";
    inputFile >> a >> b;
    cEn.push_back(a);
    cIn.push_back(b);
  }

  // inputFile.close();
  return;
}
