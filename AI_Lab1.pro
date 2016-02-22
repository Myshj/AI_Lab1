TEMPLATE = app
CONFIG += console c++11 O3
CONFIG -= app_bundle

SOURCES += main.cpp \
    test.cpp \
    ColParser.cpp \
    ColParserResponse.cpp \
    Graph.cpp \
    GraphColorer.cpp

HEADERS += \
    test.h \
    AddColorErrors.h \
    AddConnectionErrors.h \
    AddVertexErrors.h \
    ColParser.h \
    ColParserResponse.h \
    GetColorErrors.h \
    GetCountOfConflictsInVertexErrors.h \
    Graph.h \
    GraphColorer.h \
    ParseColFileErrors.h \
    SetColorErrors.h \
    graphcoloringerrors.h \
    getadjacentvertexeserrors.h \
    getworstbetweenadjacenterrors.h \
    findbestalternativecolorerrors.h

