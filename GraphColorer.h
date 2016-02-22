#ifndef GRAPHCOLORER_H
#define GRAPHCOLORER_H

#include "Graph.h"

#include "graphcoloringerrors.h"

#include "vector"

#include "set"

#include "random"

#include "math.h"

#include <iostream>

/*Performs graph coloring.*/
class GraphColorer
{
    public:
    virtual ~GraphColorer() = 0;

    static GraphColoringErrors ColorGraph(Graph& graph,
                                          const unsigned int countOfColors,
                                          const unsigned int countOfAnts,
                                          const double Pc)
    {
        /*Prepare possible colors for vertexes.*/
        for(unsigned int colorId = 0; colorId < countOfColors; colorId++)
        {
            graph.AddColor(colorId);
        }

        /*Prepare array of vertex ids for better dealing with random generator.*/
        std::vector<unsigned int> vertexIds = std::vector<unsigned int>();
        for(auto vertexId : graph.GetPossibleVertexIds())
        {
            vertexIds.push_back(vertexId);
        }

        /*Do random coloring.*/
        for(auto vertexId : vertexIds)
        {
            if(vertexId == 7){
                std::cout << "MUUU!!!" << std::endl;
            }
            graph.SetColor(vertexId, rand() % countOfColors);
        }

        unsigned int currentIteration = 1;

        /*Main loop.*/
        while(!graph.IsColoredProperly())
        {
            for(unsigned int ant = 0; ant < countOfAnts; ant++)
            {
            /*Every ant must pass through all vertexes..*/
            for(auto currentVertexId : vertexIds)
            {
                bool weWantToRecolorSomething = false;
                unsigned int vertexToRecolor = 0;

                /*Decide if we want to recolor adjacent vertex with largest count of conflicts.*/
                if(ShallAntDoRecoloringForWorstAdjacentVertex(graph, currentIteration))
                {
                    unsigned int worstAdjacentVertex = 0;

                    /*Did we found worst between adjacent vertexes?*/
                    if(graph.GetWorstBetweenAdjacent(currentVertexId, worstAdjacentVertex) == GetWorstBetweenAdjacentErrors::None)
                    {
                        weWantToRecolorSomething = true;

                        vertexToRecolor = worstAdjacentVertex;
                    }
                }
                /*Or maybe we want to recolor random vertex?*/
                else if(ShallAntDoRecoloringForRandomVertex(Pc))
                {
                    weWantToRecolorSomething = true;

                    vertexToRecolor = vertexIds[rand() % graph.GetCountOfVertexes()];
                }

                /*Do we want to recolor any vertex?*/
                if(weWantToRecolorSomething)
                {
                    /*Does this vertex really need to be recolored?*/
                    unsigned int colorOfVertexToBeRecolored = 0;
                    graph.GetColor(vertexToRecolor, colorOfVertexToBeRecolored);

                    unsigned int countOfConflictsInVertexToBeRecolored = 0;
                    graph.GetCountOfConflictsInVertex(vertexToRecolor, colorOfVertexToBeRecolored, countOfConflictsInVertexToBeRecolored);

                    /*Are there any conflicts?*/
                    if(countOfConflictsInVertexToBeRecolored == 0)
                    {
                        continue;
                    }

                    unsigned int newColor = 0;
                    graph.FindBestAlternativeColor(vertexToRecolor, newColor);

                    graph.SetColor(vertexToRecolor, newColor);
                }
            }
            }
            currentIteration++;
            std::cout << currentIteration << std::endl;
        }

        std::cout << currentIteration << std::endl;
    }

    protected:

    private:

    static bool ShallAntDoRecoloringForWorstAdjacentVertex(const Graph& graph,
                                                           const unsigned int iteration)
    {
        double avgY = 4.8*graph.GetOverallCountOfConflicts()/(graph.GetCountOfVertexes() * graph.GetCountOfVertexes());

        double avgX = 1.*graph.GetWorstCountOfConflicts();

        double pn = exp(-3.2*((5*iteration+ 1)*avgY / (avgX )));

        double r = (double)rand() / RAND_MAX;

        return r < pn;
    }

    static bool ShallAntDoRecoloringForRandomVertex(const double Pc)
    {
        double r = (double)rand() / RAND_MAX;

        return r < Pc;
    }
};

#endif // GRAPHCOLORER_H
