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

    // tallennetaan uuteen vektoriin kaupunkien id::eet ja nimet
    std::vector<nimi_id> kaupungit_jarjestyksesssa;
    for (auto& kaupunki : kaupungit)
        kaupungit_jarjestyksesssa.push_back({kaupunki.second.nimi, kaupunki.first});

    // järjestetään vektori nimen mukaan
    std::sort(kaupungit_jarjestyksesssa.begin(),kaupungit_jarjestyksesssa.end(),
              [](nimi_id tiedot1, nimi_id tiedot2)  { return tiedot1.nimi < tiedot2.nimi;} );

    // siirretään järjestetyn vektorin id::eet uuteen vektoriin ja palautetaan se
    std::vector<TownID> palaute;
    for (nimi_id& kaupunki : kaupungit_jarjestyksesssa)
        palaute.push_back(kaupunki.id);
    return palaute;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    if (town_count()==0) return std::vector<TownID> {};  // nolla kaupunkia olemassa

    // tallennetaan uuteen vektoriin kaupunkien id::eet ja etäisyys origosta
    std::vector<etaisyys_id> kaupungit_jarjestyksesssa;
    Coord origo = {0,0};
    for (auto& kaupunki : kaupungit)
        kaupungit_jarjestyksesssa.push_back(
                    {etaisyys_pisteesta(origo, kaupunki.second.koordinaatit),
                     kaupunki.first});

    // järjestetään vektori etäisyyden mukaan
    std::sort(kaupungit_jarjestyksesssa.begin(),kaupungit_jarjestyksesssa.end(),
              [](etaisyys_id tiedot1, etaisyys_id tiedot2)  {return tiedot1.etaisyys<tiedot2.etaisyys;});

    // siirretään järjestetyn vektorin id::eet uuteen vektoriin ja palautetaan se
    std::vector<TownID> palaute;
    for (etaisyys_id& kaupunki : kaupungit_jarjestyksesssa)
        palaute.push_back(kaupunki.id);
    return palaute;
}

TownID Datastructures::min_distance()
{
    if (town_count()==0) return NO_TOWNID;  // nolla kaupunkia olemassa

    etaisyys_id palaute = {2147483647, NO_TOWNID}; // kaupunkien etäisyys on pienempi kuin tämä
    Coord origo = {0,0};
    for (auto& kaupunki : kaupungit)
    {
        int etaisyys_origosta = etaisyys_pisteesta(origo, kaupunki.second.koordinaatit);
        if  (palaute.etaisyys > etaisyys_origosta)
            palaute = {etaisyys_origosta, kaupunki.first};
    }
    return palaute.id;
}

TownID Datastructures::max_distance()
{
    if (town_count()==0) return NO_TOWNID;  // nolla kaupunkia olemassa

    etaisyys_id palaute = {0, NO_TOWNID}; // kaupungin etäisyys täytyy olla suurempi kuin 0
    Coord origo = {0,0};
    for (auto& kaupunki : kaupungit)
    {
        int etaisyys_origosta = etaisyys_pisteesta(origo, kaupunki.second.koordinaatit);
        if  (palaute.etaisyys < etaisyys_origosta)
            palaute = {etaisyys_origosta, kaupunki.first};
    }
    return palaute.id;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    if (kaupungit.find(vassalid) == kaupungit.end() or kaupungit.find(masterid) == kaupungit.end()
            or kaupungit.at(vassalid).isantakaupunki != NO_TOWNID) return false;
    kaupungit.at(vassalid).isantakaupunki = masterid;
    kaupungit.at(masterid).vasalllikaupungit.push_back(vassalid);
    return true;
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

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    if (town_count()==0) return std::vector<TownID> {};  // nolla kaupunkia olemassa

    // käydään läpi kaupungit ja lähimmät kaupungit siirretään vektoriin
    std::vector<TownID> palaute;
    int pienin_etaisyys = 2147483647; // kaupunkien etäisyys on pienempi kuin tämä
    for (auto& kaupunki : kaupungit)
    {
        int etaisyys = etaisyys_pisteesta(coord, kaupunki.second.koordinaatit);
        if (pienin_etaisyys > etaisyys) // tyhjennetään lista, jos lähempi kaupunki löytyy
        {
            palaute.clear();
            palaute.push_back(kaupunki.first);
            pienin_etaisyys = etaisyys;
        }
        else if (pienin_etaisyys == etaisyys) palaute.push_back(kaupunki.first); // kaupungeilla on sama etäisyys
    }
    return palaute;
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


int Datastructures::etaisyys_pisteesta(Coord &lahto, Coord &kohde)
{
    return (int)ceil(sqrt((kohde.x - lahto.x)*(kohde.x- lahto.x)
                         + (kohde.y - lahto.y)*(kohde.y- lahto.y)));
}
