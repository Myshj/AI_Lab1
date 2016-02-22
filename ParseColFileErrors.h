#ifndef PARSECOLFILEERRORS_H_INCLUDED
#define PARSECOLFILEERRORS_H_INCLUDED

enum class ParseColFileErrors
{
    None,
    ErrorOpeningFile,
    DuplicatedGraphParameters,
    IncorrectFileStructure
};

#endif // PARSECOLFILEERRORS_H_INCLUDED
