// Student name: Tomi Lotila
// Student email: tomi.lotila@tuni.fi
// Student number: H274802

prg1 dokumentaatio (datastructures luokka)

Luokan päätietorakenne:
- tietorakenteen tulee säilyttää
    - kaupungin nimi
    - id
    - koordinaatit
    - verot
    - lista vassallikaupungeista
    - isäntäkaupunki
- kaupunkeja haetaan yleisimmin id-luvulla.
- id lukuja ei ikinä haeta järjestyksessä, joten ei tarvitse pitää järjestyksessä tietorakenteessa.
- alkioita halutaan monessa eri järjestyksessä (nimen aakkos- ja etäisyysjärjestyksssä), joten
alkiot täytyy laittaa järjestykseen haun jälkeen.
- id on jokaiselle kaupungille uniikki.
- tietorakenteeksi valitaan unordered_map, jossa id on avaimmena ja oma structi kohteena.
    - unordered_map tietorakenteesta avaimmella haku on keskimäärin vakio aikainen.
    - alkion lisäys ja poisto ovat myös keskimäärin vakio aikaisia.
- Muut tiedot säilytetään structissa.
    struct
    {
    - kaupungin nimi
    - koordinaatit
    - verot
    - lista vassallikaupungeista
    - isäntäkaupunki
    }
- Vasallikaupunkilistan tietorakenne
    - halutaan usein kaupungin kaikki vasallikaupungit
    - vasallikaupunkeja ei tarvitse säilyttää järjestyksessä (ei haeta järjestyksessä)
    - vasallikaupungin lisäys on tehokasta
    - isäntäkaupungin vasalliskaupunkeja poistetaan harvoin, joten
    poiston ei tarvitse olla tehkokas.
- Vasallikaupunkilistan tietorakenteeksi valitaan vector, sillä:
    - lisäys listan perään on vakio aikainen.
    - järjestysluvulla haku on vakio aikainen.
    - tietyn vasallikaupungin haku on lineaarinen (poistoa varten).




Luokan monimutkaisimpien metodejen pseudokoodit:


std::vector<TownID> Datastructures::towns_alphabetically()
{
    if (kaupungit.size == 0) return {};
    else
    std::vector<{Name,TownId}> kaupungit_jäjestyksessä = kaupungit; // siirretään kaupungit vektoriin
    std::sort(kaupungit_jäjestyksessä); // järjestetään vektorit nimen perusteella
    std::vector<TownID> palaute = kapunkit_järjestyksessä; // siirretään kaupunkien ID vektoriin ylläpitämällä järjestyksen
    return palaute;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{

}

TownID Datastructures::min_distance()
{

}

TownID Datastructures::max_distance()
{

}

bool Datastructures::add_vassalship(TownID /*vassalid*/, TownID /*masterid*/)
{

}

std::vector<TownID> Datastructures::get_town_vassals(TownID /*id*/)
{

}

std::vector<TownID> Datastructures::taxer_path(TownID /*id*/)
{

}

bool Datastructures::remove_town(TownID /*id*/)
{

}

std::vector<TownID> Datastructures::towns_nearest(Coord /*coord*/)
{

}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{

}

int Datastructures::total_net_tax(TownID /*id*/)
{

}


