#include "Parser.hpp"


namespace RecDescent{

Parser::Parser(std::string const &file_name){
  std::ifstream testFile("testfile", std::ios::binary);
  std::vector<char> fileContents((std::istreambuf_iterator<char>(testFile)),
                               std::istreambuf_iterator<char>());

void writeByteCode(ByteCode const &byte_code, ){
  std::unique_ptr<std::ofstream> outputFile( new std::ofstream() );
  outputFile->open (file_name);
  *outputFile << std::string("davm") << "\n";    
  *outputFile << byte_code.stream.size();
  
  for ( auto const inst : byte_code.stream){
    *outputFile << inst << "\n";
  }
  
  outputFile->close();
}  
  
}


} //end namespace RecDescent
