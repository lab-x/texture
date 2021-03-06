/*
  ToroidalDomain.cpp

  Li-Yi Wei
  August 21, 2014

*/

#include <algorithm>
using namespace std;

#include "ToroidalDomain.hpp"
#include "Exception.hpp"

ToroidalDomain::ToroidalDomain(void)
{
    // nothing else to do
}

ToroidalDomain::~ToroidalDomain(void)
{
    // nothing else to do
}

void ToroidalDomain::Correct(const Texture & source, Position & position) const
{
    if(position.size() != source.Dimension())
    {
        throw Exception("ToroidalDomain::Correct(): dimensionality mismatch");
    }

    for(unsigned int i = 0; i < position.size(); i++)
    {
        position[i] = Correct(source.Size(i), position[i]);
    }
}

void ToroidalDomain::Nearest(const Texture & source, const Position & anchor, Position & position) const
{
    if((position.size() != source.Dimension()) || (anchor.size() != source.Dimension()))
    {
        throw Exception("ToroidalDomain::Nearst(): dimensionality mismatch");
    }

    for(unsigned int i = 0; i < position.size(); i++)
    {
        position[i] = Nearest(source.Size(i), anchor[i], position[i]);
    }
}

int ToroidalDomain::Geodesic2(const Texture & source, const Position & anchor, const Position & position) const
{
    if((position.size() != source.Dimension()) || (anchor.size() != source.Dimension()))
    {
        throw Exception("ToroidalDomain::NearstDistance2(): dimensionality mismatch");
    }

    int answer = 0;

    for(unsigned int i = 0; i < anchor.size(); i++)
    {
        const int dist = Geodesic(source.Size(i), anchor[i], position[i]);

        answer += dist*dist;
    }

    return answer;
}

int ToroidalDomain::Correct(const int source, const int position)
{
    return ((position%source)+source)%source;
}

int ToroidalDomain::Nearest(const int source, const int anchor, const int position)
{
    const int answer_me = Correct(source, position);
    const int answer_neg = answer_me - source;
    const int answer_pos = answer_me + source;

    const int neg = abs(anchor - answer_neg);
    const int me = abs(anchor - answer_me);
    const int pos = abs(answer_pos - anchor);

    const int minimum = min(me, min(neg, pos));

    int answer = 0;
    if(minimum == neg)
    {
        answer = answer_neg;
    }
    else if(minimum == pos)
    {
        answer = answer_pos;
    }
    else
    {
        answer = answer_me;
    }

    return answer;
}
 
int ToroidalDomain::Geodesic(const int source, const int anchor, const int position)
{
    const int corrected = Correct(source, position);

    const int neg = abs(anchor-(corrected-source));
    const int me = abs(anchor-corrected);
    const int pos = abs(corrected+source-anchor);

    const int minimum = min(me, min(neg, pos));

    return minimum;
}
