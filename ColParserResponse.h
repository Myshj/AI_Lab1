#ifndef COLPARSERRESPONSE_H
#define COLPARSERRESPONSE_H

#include "Graph.h"

#include "memory"


class ColParserResponse
{
    public:
        ColParserResponse();

        ColParserResponse(Graph* newGraph,
                          const unsigned int newCountOfVertexes,
                          const unsigned int newCountOfEdges)
        {
            SetGraph(newGraph);

            SetCountOfVertexes(newCountOfVertexes);

            SetCountOfEdges(newCountOfEdges);
        }

        ~ColParserResponse();

        std::shared_ptr<Graph> GetGraph() const
        {
            return graph;
        }

        void SetGraph(Graph* newGraph)
        {
            graph = std::shared_ptr<Graph>(newGraph);
        }

        unsigned int GetCountOfVertexes() const
        {
            return countOfVertexes;
        }

        void SetCountOfVertexes(const unsigned int newCountOfVertexes)
        {
            countOfVertexes = newCountOfVertexes;
        }

        unsigned int GetCountOfEdges() const
        {
            return countOfEdges;
        }

        void SetCountOfEdges(const unsigned int newCountOfEdges)
        {
            countOfEdges = newCountOfEdges;
        }

    protected:

    private:

        std::shared_ptr<Graph> graph;

        unsigned int countOfVertexes;

        unsigned int countOfEdges;
};

#endif // COLPARSERRESPONSE_H
