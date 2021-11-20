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
    std::unordered_map<TownID, kaupunki_data>::iterator kaupunki = kaupungit.find(id);
    return kaupunki != kaupungit.end() ? kaupunki->second.nimi : NO_NAME;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    std::unordered_map<TownID, kaupunki_data>::iterator kaupunki = kaupungit.find(id);
    return kaupunki != kaupungit.end() ? kaupunki->second.koordinaatit : NO_COORD;
}

int Datastructures::get_town_tax(TownID id)
{
    std::unordered_map<TownID, kaupunki_data>::iterator kaupunki = kaupungit.find(id);
    return kaupunki != kaupungit.end() ? kaupunki->second.verot : NO_VALUE;
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

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    if (kaupungit.find(id) == kaupungit.end()) return std::vector<TownID> {NO_TOWNID};
    return kaupungit.at(id).vasalllikaupungit;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    if (kaupungit.find(id) == kaupungit.end()) return std::vector<TownID> {NO_TOWNID};

    std::vector<TownID> palaute ={id};
    isantakaupungit_rekursio(kaupungit.at(id).isantakaupunki, palaute);
    return palaute;

}

bool Datastructures::remove_town(TownID poistettava_kaupunki)
{
    if (kaupungit.find(poistettava_kaupunki) == kaupungit.end()) return false;

    TownID& isantakaupunki = kaupungit.at(poistettava_kaupunki).isantakaupunki;

    // kaupungilla on isäntäkaupunki, jolle siirretään vasallikaupungit
    if( isantakaupunki != NO_TOWNID)
    {
        // poistetaan poistettava vasallikaupunki isännältä
        std::vector<TownID> uudet_vasallikaupungit;
        for (TownID& isannan_vasallikaupunki : kaupungit.at(isantakaupunki).vasalllikaupungit)
            if (isannan_vasallikaupunki != poistettava_kaupunki)  uudet_vasallikaupungit.push_back(isannan_vasallikaupunki);
        kaupungit.at(isantakaupunki).vasalllikaupungit = uudet_vasallikaupungit;

        // poistettavalla kaupungilla on vasallikaupunkeja
        if (kaupungit.at(poistettava_kaupunki).vasalllikaupungit.size()!=0)
        {
            // isäntäkaupunki saa poistettavan kaupungin vasallit
            for (TownID& poistettavan_vasallikaupunki : kaupungit.at(poistettava_kaupunki).vasalllikaupungit)
            {
                kaupungit.at(poistettavan_vasallikaupunki).isantakaupunki = isantakaupunki;
                kaupungit.at(isantakaupunki).vasalllikaupungit.push_back(poistettavan_vasallikaupunki);
            }
        }
    }
    // kaupungilla ei ole isäntäkapunkia, joten vasallikaupungit vapautuvat
    else
    {
        for (TownID& vasallikaupunki : kaupungit.at(poistettava_kaupunki).vasalllikaupungit)
            kaupungit.at(vasallikaupunki).isantakaupunki = NO_TOWNID;
    }
    kaupungit.erase(poistettava_kaupunki);
    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    unsigned int town_amount = town_count();
    if (town_amount==0) return std::vector<TownID> {};  // nolla kaupunkia olemassa

    std::vector<etaisyys_id> kaupungit_jarjestyksessa;
    kaupungit_jarjestyksessa.reserve(town_amount);

    // kaupungit vektorissa
    for (auto& kaupunki : kaupungit)
        kaupungit_jarjestyksessa.push_back({etaisyys_pisteesta(coord,
                                            kaupunki.second.koordinaatit), kaupunki.first});
    // järjestetään vektori etäisyyden mukaan
    std::sort(kaupungit_jarjestyksessa.begin(),kaupungit_jarjestyksessa.end(),
              [](etaisyys_id tiedot1, etaisyys_id tiedot2)
    { return tiedot1.etaisyys < tiedot2.etaisyys;} );

    std::vector<TownID> palaute;
    palaute.reserve(town_amount);

    for (auto& kaupunki : kaupungit_jarjestyksessa)
        palaute.push_back(kaupunki.id);
    return palaute;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID kaupunki)
{
    if (kaupungit.find(kaupunki) == kaupungit.end()) return std::vector<TownID> {NO_TOWNID};

    std::vector<TownID> palaute = {kaupunki};
    vasallikaupungit_rekursio(kaupunki, palaute);
    return palaute;
}

int Datastructures::total_net_tax(TownID kaupunki)
{
    if (kaupungit.find(kaupunki) == kaupungit.end()) return NO_VALUE;

    if (kaupungit.at(kaupunki).isantakaupunki == NO_TOWNID)
        return kaupungit.at(kaupunki).verot + verotulo_rekursio(kaupunki);
    return ceil(0.9*(kaupungit.at(kaupunki).verot + verotulo_rekursio(kaupunki)));
}


int Datastructures::etaisyys_pisteesta(Coord &lahto, Coord &kohde)
{
    return (int)ceil(sqrt((kohde.x - lahto.x)*(kohde.x- lahto.x)
                         + (kohde.y - lahto.y)*(kohde.y- lahto.y)));
}


void Datastructures::isantakaupungit_rekursio(TownID id, std::vector<TownID>&kaupungit_kertyma)
{
    if(id == NO_TOWNID) return;

    kaupungit_kertyma.push_back(id);
    return isantakaupungit_rekursio(kaupungit.at(id).isantakaupunki, kaupungit_kertyma);
}


void Datastructures::vasallikaupungit_rekursio(TownID id, std::vector<TownID>&kaupungit_kertyma)
{
    std::vector<TownID>& vasallikaupungit = kaupungit.at(id).vasalllikaupungit;
    if (vasallikaupungit.empty()) return;

    // käydään läpi vasallikaupunkien vasallit ja pidetään muistissa pisintä jonoa
    std::vector<TownID> lisattavat_vasallit;
    for (auto& vasallikaupunki : vasallikaupungit)
    {
        std::vector<TownID> uudet_vasallit;
        uudet_vasallit.push_back(vasallikaupunki);
        vasallikaupungit_rekursio(vasallikaupunki, uudet_vasallit);
        if (lisattavat_vasallit.size() < uudet_vasallit.size()) lisattavat_vasallit = uudet_vasallit;
    }
    // yhdistetään pisimmän jonon vasallit
    for (auto& vasalli : lisattavat_vasallit)
        kaupungit_kertyma.push_back(vasalli);
    return;
}

int Datastructures::verotulo_rekursio(TownID id)
{
    int isannan_verot = 0;
    for (auto& vasallikaupunki : kaupungit.at(id).vasalllikaupungit)
    {
        isannan_verot += floor(0.1*(kaupungit.at(vasallikaupunki).verot
                              + verotulo_rekursio(vasallikaupunki)));
    }
    return isannan_verot;
}




