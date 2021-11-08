// Datastructures.cc
//
// Student name: Tomi Lotila
// Student email: tomi.lotila@tuni.fi
// Student number: H274802

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <algorithm>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures() :
    kaupungit()
{

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::town_count()
{
   return kaupungit.size();
}

void Datastructures::clear_all()
{
    if (town_count()==0) return;
    kaupungit.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    if (kaupungit.find(id) != kaupungit.end()) return false;
    kaupungit.insert({id, {name, coord, tax, {}, NO_TOWNID}});
    return true;
}

Name Datastructures::get_town_name(TownID id)
{
    return kaupungit.find(id) != kaupungit.end() ? kaupungit.find(id)->second.nimi : NO_NAME;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    return kaupungit.find(id) != kaupungit.end() ? kaupungit.find(id)->second.koordinaatit : NO_COORD;
}

int Datastructures::get_town_tax(TownID id)
{
    return kaupungit.find(id) != kaupungit.end() ? kaupungit.find(id)->second.verot : NO_VALUE;
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> kaikki_kaupungit;
    for (auto& kaupunki : kaupungit )
        kaikki_kaupungit.push_back(kaupunki.first);
    return kaikki_kaupungit;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> halutut_kaupungit;
    for (auto& kaupunki : kaupungit )
        if (kaupunki.second.nimi == name) halutut_kaupungit.push_back(kaupunki.first);
    return halutut_kaupungit;
}

bool Datastructures::change_town_name(TownID id, const Name & newname)
{
    if (get_town_name(id) == NO_NAME) return false;
    kaupungit.find(id)->second.nimi = newname;
    return true;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    if (town_count()==0) return std::vector<TownID> {};  // nolla kaupunkia olemassa

    std::vector<nimi_id> kaupungit_jarjestyksesssa;
    for (auto& kaupunki : kaupungit)
        kaupungit_jarjestyksesssa.push_back({kaupunki.second.nimi, kaupunki.first});

    std::sort(kaupungit_jarjestyksesssa.begin(),kaupungit_jarjestyksesssa.end(),
              [](nimi_id tiedot1, nimi_id tiedot2)  { return tiedot1.nimi < tiedot2.nimi;} );

    std::vector<TownID> palaute;
    for (nimi_id& kaupunki : kaupungit_jarjestyksesssa)
        palaute.push_back(kaupunki.id);

    return palaute;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // Replace the line below with your implementation
    throw NotImplemented("towns_distance_increasing()");
}

TownID Datastructures::min_distance()
{
    // Replace the line below with your implementation
    throw NotImplemented("min_distance()");
}

TownID Datastructures::max_distance()
{
    // Replace the line below with your implementation
    throw NotImplemented("max_distance()");
}

bool Datastructures::add_vassalship(TownID /*vassalid*/, TownID /*masterid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("add_vassalship()");
}

std::vector<TownID> Datastructures::get_town_vassals(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("get_town_vassals()");
}

std::vector<TownID> Datastructures::taxer_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("taxer_path()");
}

bool Datastructures::remove_town(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_town()");
}

std::vector<TownID> Datastructures::towns_nearest(Coord /*coord*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("towns_nearest()");
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("total_net_tax()");
}
