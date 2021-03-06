// Datastructures.hh
//
// Student name: Tomi Lotila
// Student email: tomi.lotila@tuni.fi
// Student number: H274802

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: unordered_map.size() metodi on vako aikainen.
    unsigned int town_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: kutsutaan unordered_map.clear() metodia, joka on lineaarinen.
    void clear_all();

    // Estimate of performance: theta(1), O(n)
    // Short rationale for estimate: unordered_map haku ja lis??ys ovat keskim????rin vakio aikaisia.
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: theta(1), O(n)
    // Short rationale for estimate: unordered_map haku on keskim????rin vakio aikainen.
    Name get_town_name(TownID id);

    // Estimate of performance: theta(1), O(n)
    // Short rationale for estimate: unordered_map haku on keskim????rin vakio aikainen.
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: theta(1), O(n)
    // Short rationale for estimate: unordered_map haku on keskim????rin vakio aikainen.
    int get_town_tax(TownID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: tietorakenteen luuppaaminen on lineaarinen toiminto.
    // Loopin sis??ll?? alkion lis??ys vektorin loppuun on vakio aikainen, joten ei vaikuta asymptoosiseen tehokkuuteen.
    std::vector<TownID> all_towns();

    // Estimate of performance: O(n)
    // Short rationale for estimate: tietorakenteen luuppaaminen on lineaarinen toiminto.
    // Loopin sis??ll?? alkion lis??ys vektorin loppuun on vakio aikainen, joten ei vaikuta asymptoosiseen tehokkuuteen.
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: O(1)
    // Short rationale for estimate: get_tow_name() ja unordered_map.find() metodit ovat vakio aikaisia.
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance:  O(nlog(n))
    // Short rationale for estimate: for-luuppien tehokkuus on O(n) ja std::sort algoritmin tehokkuus on O(nlog(n)).
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(nlog(n))
    // Short rationale for estimate: for-luuppien tehokkuus on O(n) ja std::sort algoritmin tehokkuus on O(nlog(n)).
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-looppi on lineaarinen
    TownID min_distance();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-looppi on lineaarinen
    TownID max_distance();

    // Estimate of performance: O(1)
    // Short rationale for estimate: unordered_map.find() on keskim????rin vakio aikainen
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: O(1)
    // Short rationale for estimate: unordered_map.find() on keskim????rin vakio aikainen
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: omega(1), O(n)
    // Short rationale for estimate: parhaimmassa tapauksessa vakio, jos ei is??nt??kaupunkeja.
    // huonoimmassa tapauksessa k??yd????n l??pi kaikki kaupungit, jolloin lineaarinen
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-silmukat ovat lineaariset ja unordered_map.erase() on lineaarinen
    bool remove_town(TownID id);

    // Estimate of performance:  O(nlog(n))
    // Short rationale for estimate: for-luuppien tehokkuus on O(n) ja std::sort algoritmin tehokkuus on O(nlog(n)).
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: omega(1), O(n)
    // Short rationale for estimate: Parhaimmassa tapauksessa vakio, kun ei ole vasallikaupunkeja.
    // huonoimmassa tapauksessa lineaarinen, jossa kaikki kaupungit ovat tutkittavan kaupungin vasalleja.
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: omega(1), O(n)
    // Short rationale for estimate: Parhaimmassa tapauksessa vakio, kun ei ole vasallikaupunkeja.
    // huonoimmassa tapauksessa lineaarinen, jossa kaikki kaupungit ovat tutkittavan kaupungin vasalleja.
    int total_net_tax(TownID id);

private:
    struct kaupunki_data
    {
        Name nimi;
        Coord koordinaatit;
        int verot;
        std::vector<TownID> vasalllikaupungit;
        TownID isantakaupunki;
    };

    std::unordered_map<TownID, kaupunki_data> kaupungit;

    struct nimi_id
    {
        Name nimi;
        TownID id;
    };
    struct etaisyys_id
    {
        int etaisyys;
        TownID id;
    };

    int etaisyys_pisteesta(Coord &lahto, Coord &kohde);

    void isantakaupungit_rekursio(TownID id, std::vector<TownID>&kaupungit_kertyma);

    void vasallikaupungit_rekursio(TownID id, std::vector<TownID>&kaupungit_kertyma);

    int verotulo_rekursio(TownID id);
};

#endif // DATASTRUCTURES_HH
