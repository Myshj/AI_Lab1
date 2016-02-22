#ifndef COLPARSER_H
#define COLPARSER_H

#include "string"
#include "vector"

#include "ColParserResponse.h"

#include "ParseColFileErrors.h"

#include "iostream"
#include "fstream"
#include "sstream"

class ColParser
{
    public:
        virtual ~ColParser() = 0;

        /*
        Returns information from parsed file in parameter outResponse if success.
        Returns default object otherwise.
        */
        static ParseColFileErrors ParseColFile(const std::string& filename,
                                               ColParserResponse& outResponse)
        {
            outResponse = ColParserResponse();

            /*Try to open file with specified filename.*/
            std::ifstream fs; //file stream to read from
            fs.open(filename);
            /*Return quickly if something went wrong.*/
            if(!fs.is_open())
            {
                fs.close();

                return ParseColFileErrors::ErrorOpeningFile;
            }

            /*Main loop.*/
            bool wereParameters = false; // Did we read graph parameters already?
            std::string line = "";
            auto retGraph = new Graph();
            while(std::getline(fs, line))
            {
                /*
                Split our string.
                Now we have an array of words.
                */
                std::istringstream iss(line, std::istringstream::in);
                std::string word;
                std::vector<std::string> words;
                while(iss >> word)
                {
                    words.push_back(word);
                }

                /*Do we have an empty string?*/
                if(words.empty())
                {
                    continue;
                }

                /*Is there a comment string?*/
                if(words[0].compare(BEGINNING_OF_COMMENT) == 0)
                {
                    continue;
                }

                /*Is there a string with parameters?*/
                if(words[0].compare(BEGINNING_OF_PARAMETERS) == 0)
                {
                    /*Did we already read parameters?*/
                    if(wereParameters)
                    {
                        fs.close();

                        delete retGraph;

                        return ParseColFileErrors::DuplicatedGraphParameters;
                    }

                    /*Do we have malformed string?*/
                    if(words.size() != 4)
                    {
                        fs.close();

                        delete retGraph;

                        return ParseColFileErrors::IncorrectFileStructure;
                    }


                    /*Read parameters.*/
                    try
                    {
                        unsigned int firstParameter = std::stoul(words[2]);
                        unsigned int secondParameter = std::stoul(words[3]);

                        outResponse.SetCountOfVertexes(firstParameter);
                        outResponse.SetCountOfEdges(secondParameter);
                    }
                    /*Were parameters malformed?*/
                    catch(...)
                    {
                        fs.close();

                        delete retGraph;

                        return ParseColFileErrors::IncorrectFileStructure;
                    }

                    /*Record that we have already read graph parameters.*/
                    wereParameters = true;

                    continue;
                }

                /*Is there a string with edge definition?*/
                if(words[0].compare(BEGINNING_OF_EDGE) == 0)
                {
                    /*Do we have malformed string?*/
                    if(words.size() != 3)
                    {
                        fs.close();

                        delete retGraph;

                        return ParseColFileErrors::IncorrectFileStructure;
                    }

                    /*Read parameters.*/
                    try
                    {
                        unsigned int firstParameter = std::stoul(words[1]);
                        unsigned int secondParameter = std::stoul(words[2]);

                        retGraph->AddVertex(firstParameter);
                        retGraph->AddVertex(secondParameter);

                        retGraph->AddConnection(firstParameter, secondParameter);
                        retGraph->AddConnection(secondParameter, firstParameter);
                    }
                    catch(...)
                    {
                        fs.close();

                        delete retGraph;

                        return ParseColFileErrors::IncorrectFileStructure;
                    }
                }

            }

            outResponse.SetGraph(retGraph);

            fs.close();

            return ParseColFileErrors::None;
        }

    protected:

    private:

        static const std::string BEGINNING_OF_COMMENT;// = "c";

        static const std::string BEGINNING_OF_PARAMETERS;// = "p";

        static const std::string BEGINNING_OF_EDGE;// = "e";
};

#endif // COLPARSER_H
