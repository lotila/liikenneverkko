// Datastructures.cc
//
// Student name: Tomi Lotila
// Student email: tomi.lotila@tuni.fi
// Student number: H274802

#include "datastructures.hh"

#include <random>

#include <cmath>

#include <algorithm>

#include <stack>


std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator


template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}


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
    clear_roads();
    kaupungit.clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    if (kaupungit.find(id) != kaupungit.end()) return false;
    kaupungit.insert({id, {name, coord, tax, {}, NO_TOWNID, {}}});
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
    unsigned int town_amount= town_count();
    if (town_amount==0) return std::vector<TownID> {};  // nolla kaupunkia olemassa

    // tallennetaan uuteen vektoriin kaupunkien id::eet ja nimet
    std::vector<nimi_id> kaupungit_jarjestyksesssa;
    kaupungit_jarjestyksesssa.reserve(town_amount);

    for (auto& kaupunki : kaupungit)
        kaupungit_jarjestyksesssa.push_back({kaupunki.second.nimi, kaupunki.first});

    // j??rjestet????n vektori nimen mukaan
    std::sort(kaupungit_jarjestyksesssa.begin(),kaupungit_jarjestyksesssa.end(),
              [](nimi_id tiedot1, nimi_id tiedot2)  { return tiedot1.nimi < tiedot2.nimi;} );

    // siirret????n j??rjestetyn vektorin id::eet uuteen vektoriin ja palautetaan se
    std::vector<TownID> palaute;
    palaute.reserve(town_amount);
    for (nimi_id& kaupunki : kaupungit_jarjestyksesssa)
        palaute.push_back(kaupunki.id);
    return palaute;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    unsigned int town_amount= town_count();
    if (town_amount==0) return std::vector<TownID> {};  // nolla kaupunkia olemassa

    // tallennetaan uuteen vektoriin kaupunkien id::eet ja et??isyys origosta
    std::vector<etaisyys_id> kaupungit_jarjestyksesssa;
    kaupungit_jarjestyksesssa.reserve(town_amount);

    Coord origo = {0,0};
    for (auto& kaupunki : kaupungit)
        kaupungit_jarjestyksesssa.push_back(
                    {etaisyys_pisteesta(origo, kaupunki.second.koordinaatit),
                     kaupunki.first});

    // j??rjestet????n vektori et??isyyden mukaan
    std::sort(kaupungit_jarjestyksesssa.begin(),kaupungit_jarjestyksesssa.end(),
              [](etaisyys_id tiedot1, etaisyys_id tiedot2)  {return tiedot1.etaisyys<tiedot2.etaisyys;});

    // siirret????n j??rjestetyn vektorin id::eet uuteen vektoriin ja palautetaan se
    std::vector<TownID> palaute;
    palaute.reserve(town_amount);
    for (etaisyys_id& kaupunki : kaupungit_jarjestyksesssa)
        palaute.push_back(kaupunki.id);
    return palaute;
}

TownID Datastructures::min_distance()
{
    if (town_count()==0) return NO_TOWNID;  // nolla kaupunkia olemassa

    etaisyys_id palaute = {2147483647, NO_TOWNID}; // kaupunkien et??isyys on pienempi kuin t??m??
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

    etaisyys_id palaute = {0, NO_TOWNID}; // kaupungin et??isyys t??ytyy olla suurempi kuin 0
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

    // kaupungilla on is??nt??kaupunki, jolle siirret????n vasallikaupungit
    if( isantakaupunki != NO_TOWNID)
    {
        // poistetaan poistettava vasallikaupunki is??nn??lt??
        std::vector<TownID> uudet_vasallikaupungit;
        for (TownID& isannan_vasallikaupunki : kaupungit.at(isantakaupunki).vasalllikaupungit)
            if (isannan_vasallikaupunki != poistettava_kaupunki)  uudet_vasallikaupungit.push_back(isannan_vasallikaupunki);
        kaupungit.at(isantakaupunki).vasalllikaupungit = uudet_vasallikaupungit;

        // poistettavalla kaupungilla on vasallikaupunkeja
        if (kaupungit.at(poistettava_kaupunki).vasalllikaupungit.size()!=0)
        {
            // is??nt??kaupunki saa poistettavan kaupungin vasallit
            for (TownID& poistettavan_vasallikaupunki : kaupungit.at(poistettava_kaupunki).vasalllikaupungit)
            {
                kaupungit.at(poistettavan_vasallikaupunki).isantakaupunki = isantakaupunki;
                kaupungit.at(isantakaupunki).vasalllikaupungit.push_back(poistettavan_vasallikaupunki);
            }
        }
    }
    // kaupungilla ei ole is??nt??kapunkia, joten vasallikaupungit vapautuvat
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
    // j??rjestet????n vektori et??isyyden mukaan
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
    return (int)floor(sqrt((kohde.x - lahto.x)*(kohde.x- lahto.x)
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

    // k??yd????n l??pi vasallikaupunkien vasallit ja pidet????n muistissa pisint?? jonoa
    std::vector<TownID> lisattavat_vasallit;
    for (auto& vasallikaupunki : vasallikaupungit)
    {
        std::vector<TownID> uudet_vasallit;
        uudet_vasallit.push_back(vasallikaupunki);
        vasallikaupungit_rekursio(vasallikaupunki, uudet_vasallit);
        if (lisattavat_vasallit.size() < uudet_vasallit.size()) lisattavat_vasallit = uudet_vasallit;
    }
    // yhdistet????n pisimm??n jonon vasallit
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

//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    for (auto& kaupunki : kaupungit)
        kaupunki.second.naapurit.clear();
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::vector<std::pair<TownID, TownID>> tiet;

    for (auto& kaupunki : kaupungit)
        for (auto& naapuri : kaupunki.second.naapurit)
            if (kaupunki.first < naapuri)
                tiet.push_back({kaupunki.first, naapuri});

    return tiet;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    auto& kaupungin_1_naapurit = kaupungit.at(town1).naapurit;
    auto& kaupungin_2_naapurit = kaupungit.at(town2).naapurit;

   if (kaupungit.end()== kaupungit.find(town1) //kaupunkia 1 ei l??ydy
           or kaupungit.end()== kaupungit.find(town2) // kaupunkia 2 ei l??ydy
           or kaupungin_1_naapurit.end()
           != kaupungin_1_naapurit.find((town2))) // kaupunkien v??ill?? on jo tie
       return false;

   kaupungin_1_naapurit.insert(town2);
   kaupungin_2_naapurit.insert(town1);
   return true;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    if( kaupungit.end() == kaupungit.find(id)) return {NO_TOWNID};

    auto& naapurit = kaupungit.at(id).naapurit;
    std::vector<TownID> palaute;
    palaute.reserve(naapurit.size());

    for (auto& naapuri : naapurit) palaute.push_back(naapuri);
    return palaute;
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    if (kaupungit.end()== kaupungit.find(fromid) //kaupunkia 1 ei l??ydy
            or kaupungit.end()== kaupungit.find(toid)) // kaupunkia 2 ei l??ydy
        return {NO_TOWNID};

    return DFS_etsii_reitin(fromid, toid);
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    auto& kaupungin_1_naapurit = kaupungit.at(town1).naapurit;
    auto& kaupungin_2_naapurit = kaupungit.at(town2).naapurit;

    if (kaupungit.end()== kaupungit.find(town1) //kaupunkia 1 ei l??ydy
            or kaupungit.end()== kaupungit.find(town2) // kaupunkia 2 ei l??ydy
            or kaupungin_1_naapurit.end()
            == kaupungin_1_naapurit.find((town2))) // kaupunkien v??ill?? ei ole tiet??
        return false;

    kaupungin_1_naapurit.erase(town2);
    kaupungin_2_naapurit.erase(town1);
    return true;
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    if (kaupungit.end()== kaupungit.find(fromid) //kaupunkia 1 ei l??ydy
            or kaupungit.end()== kaupungit.find(toid)) // kaupunkia 2 ei l??ydy
        return {NO_TOWNID};

    return BFS_etsii_reitin(fromid, toid);
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    if (kaupungit.end()== kaupungit.find(startid)) //kaupunkia 1 ei l??ydy
        return {NO_TOWNID};

    // reitin varella olleet kaupungit
    std::unordered_map<TownID, DFS_jaljitus_tiedot> kaudut_kaupungit;
    for (auto& kaupunki : kaupungit) kaudut_kaupungit.insert({kaupunki.first, {NO_TOWNID, WHITE}});

    std::stack<TownID> kautavat_kaupungit;
    kautavat_kaupungit.push(startid);
    TownID kaupunki;
    TownID viimenen_kaupunki = NO_TOWNID;
    bool looppi = true;
    while(kautavat_kaupungit.size() != 0 and looppi)
    {
        kaupunki = kautavat_kaupungit.top();
        kautavat_kaupungit.pop();
        if (kaudut_kaupungit.at(kaupunki).vari == WHITE )
        {
            kaudut_kaupungit.at(kaupunki).vari = GRAY;
            kautavat_kaupungit.push(kaupunki);
            for(auto& naapuri : kaupungit.at(kaupunki).naapurit)
            {
                if (kaudut_kaupungit.at(naapuri).vari == WHITE)
                {
                    kautavat_kaupungit.push(naapuri);
                    kaudut_kaupungit.at(naapuri).paluu = kaupunki;
                }
                else if (kaudut_kaupungit.at(naapuri).vari == GRAY and
                         naapuri != kaudut_kaupungit.at(kaupunki).paluu)
                {
                    viimenen_kaupunki = naapuri;
                    looppi = false;
                    break;
                }
            }

        }
        else kaudut_kaupungit.at(kaupunki).vari = BLACK;
    }
    if (looppi) return {}; // sykli?? ei l??ytynyt

    // j??rjestet????n reitti vektoriin ja palautetaan
    std::vector<TownID> reitti {viimenen_kaupunki};

    while (kaupunki != NO_TOWNID)
    {
        reitti.push_back(kaupunki);
        kaupunki = kaudut_kaupungit.at(kaupunki).paluu;
    }
    std::reverse(reitti.begin(), reitti.end());
    return reitti;
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    // reitin varella olleet kaupungit
    std::unordered_map<TownID, BFS_jaljitus_tiedot> kaudut_kaupungit;
    for (auto& kaupunki : kaupungit) kaudut_kaupungit.insert({kaupunki.first, {NO_TOWNID, WHITE, 2147483647}});

    std::set<std::pair<int, TownID>> kautavat_kaupungit;

    kautavat_kaupungit.insert({0,fromid});
    kaudut_kaupungit.at(fromid).vari = GRAY;
    kaudut_kaupungit.at(fromid).etaisyys = 0;

    while(kautavat_kaupungit.size() != 0)
    {
        std::pair<int, TownID> kaupunki = *kautavat_kaupungit.begin();
        kautavat_kaupungit.erase(kautavat_kaupungit.begin());

        for (auto& naapuri : kaupungit.at(kaupunki.second).naapurit)
        {
            int etaisyys = kaudut_kaupungit.at(kaupunki.second).etaisyys + etaisyys_pisteesta(kaupungit.at(kaupunki.second).koordinaatit,
                                                                                            kaupungit.at(naapuri).koordinaatit);
            bool hinta_pienini = false;
            if( kaudut_kaupungit.at(naapuri).etaisyys > etaisyys)
            {
                kaudut_kaupungit.at(naapuri).paluu = kaupunki.second;
                kaudut_kaupungit.at(naapuri).etaisyys = etaisyys;
                hinta_pienini = true;

            }
            if (kaudut_kaupungit.at(naapuri).vari == WHITE)
            {
                kaudut_kaupungit.at(naapuri).vari = GRAY;
                kautavat_kaupungit.insert({etaisyys,naapuri});
            }
            else if (hinta_pienini)
            {
                kautavat_kaupungit.insert({etaisyys,naapuri});
            }
        }
    }
    if (kaudut_kaupungit.at(toid).etaisyys == 2147483647) return {}; // reitti?? ei l??ytynyt

    //j??rjestet????n reitti vektoriin ja palautetaan
    std::vector<TownID> reitti;
    while (toid != NO_TOWNID)
    {
        reitti.push_back(toid);
        toid = kaudut_kaupungit.at(toid).paluu;
    }
    std::reverse(reitti.begin(), reitti.end());
    return reitti;
}

Distance Datastructures::trim_road_network()
{
    std::set<std::pair<int, std::pair<TownID, TownID>>> tiet_jarjetyksessa; // pituus j??rjestyksess??
    std::vector<std::pair<TownID, TownID>> tiet_sekasin = all_roads();

    for (auto& tie : tiet_sekasin)
      {
          tiet_jarjetyksessa.insert({etaisyys_pisteesta(kaupungit.at(tie.first).koordinaatit, kaupungit.at(tie.second).koordinaatit), {tie.first, tie.second}});
      }

    int teiden_pituus = 0;
    TownID eka_loutu = NO_TOWNID;
    TownID toka_loutu = NO_TOWNID;
    std::unordered_map<TownID,std::unordered_set<TownID>> valitut_kaupungit;

    for (auto& tie : tiet_jarjetyksessa)
    {
        eka_loutu = NO_TOWNID;
        toka_loutu = NO_TOWNID;
        for (auto& kaupunki : valitut_kaupungit)
        {
            if (kaupunki.second.find(tie.second.first) != kaupunki.second.end())
            {
                eka_loutu = kaupunki.first;
            }
            if (kaupunki.second.find(tie.second.second) != kaupunki.second.end())
            {
                toka_loutu = kaupunki.first;
            }
        }
        if (eka_loutu == NO_TOWNID and toka_loutu == NO_TOWNID)
        {
            teiden_pituus += tie.first;
            valitut_kaupungit.insert({tie.second.first, {tie.second.first, tie.second.second}});
        }
        else if (eka_loutu == NO_TOWNID)
        {
            teiden_pituus += tie.first;
            valitut_kaupungit.at(toka_loutu).insert(tie.second.first);
        }
        else if (toka_loutu == NO_TOWNID)
        {
            teiden_pituus += tie.first;
            valitut_kaupungit.at(eka_loutu).insert(tie.second.second);
        }
        else if (eka_loutu == toka_loutu)
        {
            remove_road(tie.second.second, tie.second.first);
        }
        else
        {
            teiden_pituus += tie.first;
            valitut_kaupungit.at(eka_loutu).insert(
                        valitut_kaupungit.at(toka_loutu).begin(),valitut_kaupungit.at(toka_loutu).end());
            valitut_kaupungit.erase(toka_loutu);
        }
    }
    return teiden_pituus;
}


std::vector<TownID> Datastructures::DFS_etsii_reitin(TownID& fromid, TownID& toid)
{
    // reitin varella olleet kaupungit
    std::unordered_map<TownID, DFS_jaljitus_tiedot> kaudut_kaupungit;
    for (auto& kaupunki : kaupungit) kaudut_kaupungit.insert({kaupunki.first, {NO_TOWNID, WHITE}});

    std::stack<TownID> kautavat_kaupungit;
    kautavat_kaupungit.push(fromid);
    TownID kaupunki;
    while(kautavat_kaupungit.size() != 0 and kaupunki != toid)
    {
        kaupunki = kautavat_kaupungit.top();
        kautavat_kaupungit.pop();
        if (kaudut_kaupungit.at(kaupunki).vari == WHITE)
        {
            kaudut_kaupungit.at(kaupunki).vari = GRAY;
            kautavat_kaupungit.push(kaupunki);
            for(auto& naapuri : kaupungit.at(kaupunki).naapurit)
            {
                if (kaudut_kaupungit.at(naapuri).vari == WHITE)
                {
                    kautavat_kaupungit.push(naapuri);
                    kaudut_kaupungit.at(naapuri).paluu = kaupunki;
                }
            }
        }
    }
    if (kaupunki != toid) return {}; // reitti?? ei l??ytynyt

    // j??rjestet????n reitti vektoriin ja palautetaan
    std::vector<TownID> reitti;
    kaupunki = toid;
    while (kaupunki != NO_TOWNID)
    {
        reitti.push_back(kaupunki);
        kaupunki = kaudut_kaupungit.at(kaupunki).paluu;
    }
    std::reverse(reitti.begin(), reitti.end());
    return reitti;
}

std::vector<TownID> Datastructures::BFS_etsii_reitin(TownID& fromid, TownID& toid)
{
    // reitin varella olleet kaupungit
    std::unordered_map<TownID, BFS_jaljitus_tiedot> kaudut_kaupungit;
    for (auto& kaupunki : kaupungit) kaudut_kaupungit.insert({kaupunki.first, {NO_TOWNID, WHITE, 0}});

    std::deque<TownID> kautavat_kaupungit;
    kautavat_kaupungit.push_back(fromid);
    kaudut_kaupungit.at(fromid).vari = GRAY;
    TownID kaupunki;
    while(kautavat_kaupungit.size() != 0)
    {
        kaupunki = kautavat_kaupungit.front();
        kautavat_kaupungit.pop_front();
        for (auto& naapuri : kaupungit.at(kaupunki).naapurit)
        {
            if (kaudut_kaupungit.at(naapuri).vari == WHITE)
            {
                kaudut_kaupungit.at(naapuri) = {kaupunki, GRAY,
                        kaudut_kaupungit.at(kaupunki).etaisyys + 1};
                kautavat_kaupungit.push_back(naapuri);
            }
        }
    }
    if (kaudut_kaupungit.at(toid).etaisyys == 0) return {}; // reitti?? ei l??ytynyt

    // j??rjestet????n reitti vektoriin ja palautetaan
    std::vector<TownID> reitti;
    kaupunki = toid;
    while (kaupunki != NO_TOWNID)
    {
        reitti.push_back(kaupunki);
        kaupunki = kaudut_kaupungit.at(kaupunki).paluu;
    }
    std::reverse(reitti.begin(), reitti.end());
    return reitti;
}

