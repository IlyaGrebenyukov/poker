#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iterator>
#include <string>


class Card {
  public:
    enum Rank {
      Two,
      Three,
      Four,
      Five,
      Six,
      Seven,
      Eight,
      Nine,
      Ten,
      Jack,
      Queen,
      King,
      Ace
    };
    
    enum Suit {
      Hearts,
      Clubs,
      Diamonds,
      Spades,
    };
    
    Rank rank;
    Suit suit;
    
    Card (Rank r, Suit s) : rank(r), suit(s) {}
    
    static bool compare(const Card& a, const Card& b) {
      return a.rank > b.rank;
    }
};

std::ostream& operator<<(std::ostream& os, const Card& card) {
  static const std::string rankNames[] = {
    " 2", " 3", " 4", " 5",
    " 6", " 7", " 8", " 9", "10",
    " J", " Q", " K", " A"
  };
  static const std::string suitNames[] = {
    "❤", "♣", "♦", "♠︎"
  };
  os << rankNames[(int)card.rank] << " " << suitNames[(int)card.suit];
  return os;
}

static bool hasHigherRank(Card& a, Card& b) {
  return a.rank > b.rank;
}

class Deck {
  public:
    enum Type {
      Empty,
      Standard
    };
    std::vector<Card> cards;
    
    Deck (Type t = Type::Empty) {
      if (t == Type::Standard) {
        for (int r = 0; r < 13; r += 1) {
          for (int s = 0; s < 4; s += 1) {
            Card::Rank rank = (Card::Rank)(r);
            Card::Suit suit = (Card::Suit)(s);
            add(rank, suit);
          }
        }
      }
    }
    
    void add(Card::Rank rank, Card::Suit suit) {
      cards.push_back(Card(rank, suit));
    }
    
    void shuffle(int count) {
      if (cards.size()) {
        std::random_device rd;
        std::mt19937 g(rd());
        for (int i = 0; i < count; i += 1) {
          std::shuffle(cards.begin(), cards.end(), g);
        }
      }
    }
    
    void sort() {
      std::stable_sort(cards.begin(), cards.end(), Card::compare);
      std::stable_sort(cards.begin(), cards.end(), Card::compare);
      
    }
    
    void log() {
      std::copy(cards.begin(), cards.end(), std::ostream_iterator<Card>(std::cout, "\n"));
      std::cout << std::endl;
    }
};

class Hand {
  public:
    enum Rank {
      HighCard,
      Pair,
      TwoPair,
      ThreeOfAKind,
      Straight,
      Flush,
      FullHouse,
      FourOfAKind,
      StraightFlush,
      RoyalFlush
    };
    
    Deck deck;
    Rank rank;
    
    Hand () : rank(Rank::HighCard) {
      
    }
    
    inline bool operator<(const Hand& other) {
      return rank < other.rank;
    }
    
    inline bool operator>(const Hand& other) {
      return rank > other.rank;
    }
    
    void add(Card card) {
      deck.add(card.rank, card.suit);
      rank = determineRanking();
    }
    
    Rank determineRanking() {
      if (deck.cards.size() < 5) {
        return Rank::HighCard;
      }
      
      deck.sort();
      
      int ranks[13] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0
      };
      
      int oak2[13] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0
      };
      
      int oak3[13] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0
      };
      
      int oak4[13] = {
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0
      };
      
      int suits[4] = { 0, 0, 0, 0 };
      
      bool has4 = false;
      bool has3 = false;
      bool has2 = false;
      int pairs = 0;
      
      bool inSequence = true;
      
      for (auto card : deck.cards) {
        ranks[(int)card.rank] += 1;
        suits[(int)card.suit] += 1;
      }
      
      for (int i = 0; i < 4; i += 1) {
        Card& a = deck.cards.at(i);
        Card& b = deck.cards.at(i + 1);
        int d = (int)a.rank - (int)b.rank;
        // std::cout << a << " <> " << b << std::endl << d << std::endl;
        if (d != 1) {
          inSequence = false;
          break;
        }
      }
      
      for (int i = 0; i < 13; i += 1) {
        pairs += ~~(ranks[i] / 2);
        if (ranks[i] == 4) {
          has4 = true;
          oak4[i] += 1;
        }
        else if (ranks[i] == 3) {
          has3 = true;
          oak3[i] += 1;
        }
        else if (ranks[i] == 2) {
          oak2[i] += 1;
          has2 = true;
        }
      }
      
#if 0
      {
        std::cout << std::endl;
        std::cout << "        2  3  4  5  6  7  8  9  T  J  Q  K  A" << std::endl;
        std::cout << "Ranks: ";
        for (int i = 0; i < 13; i += 1) {
          std::cout << "[" << ranks[i] << "]";
        }
        std::cout << std::endl << "       ";
        for (int i = 0; i < 13; i += 1) {
          std::cout << "[" << oak4[i] << "]";
        }
        std::cout << std::endl << "       ";
        for (int i = 0; i < 13; i += 1) {
          std::cout << "[" << oak3[i] << "]";
        }
        std::cout << std::endl << "       ";
        for (int i = 0; i < 13; i += 1) {
          std::cout << "[" << oak2[i] << "]";
        }
        
        std::cout << std::endl << std::endl;
        std::cout << "        ❤  ♣  ♦  ♠" << std::endl;
        std::cout << "Suits: ";
        for (int i = 0; i < 4; i += 1) {
          std::cout << "[" << suits[i] << "]";
        }
        std::cout << std::endl;
        std::cout << "Pairs: " << pairs << std::endl;
        std::cout << "4: " << (has4 ? "Yes" : "No") << std::endl;
        std::cout << "3: " << (has3 ? "Yes" : "No") << std::endl;
        std::cout << "Sequential: " << (inSequence ? "Yes" : "No") << std::endl;
        std::cout << std::endl;
      }
#endif
      
      // do we have a royal flush?
      // A K Q J 10 same suit
      if (suits[(int)deck.cards.at(0).suit] == 5) {
        // all same suit
        if (
          ranks[Card::Rank::Ace] == 1 &&
          ranks[Card::Rank::King] == 1 &&
          ranks[Card::Rank::Queen] == 1 &&
          ranks[Card::Rank::Jack] == 1 &&
          ranks[Card::Rank::Ten] == 1) {
          // boom baby
          return Rank::RoyalFlush;
        }
      }
      
      // do we have a straight flush?
      if (inSequence && suits[(int)deck.cards.at(0).suit] == 5) {
        return Rank::StraightFlush;
      }
      
      // do we have four of a kind?
      if (has4) {
        return Rank::FourOfAKind;
      }
      
      auto sumOf = [](int count, int* items) {
        int sum = 0;
        for (int i = 0; i < count; i += 1) {
          sum += items[i];
        }
        return sum;
      };
      
      // do we have a full house?
      if (has3 && sumOf(13, oak2) == 1) {
        return Rank::FullHouse;
      }
      
      // do we have a flush?
      if (!inSequence && suits[(int)deck.cards.at(0).suit] == 5) {
        return Rank::Flush;
      }
      
      // do we have a straight?
      if (inSequence && suits[(int)deck.cards.at(0).suit] != 5) {
        return Rank::Straight;
      }
      
      // do we have three of a kind?
      if (has3) {
        return Rank::ThreeOfAKind;
      }
      
      // do we have two pair?
      if (pairs == 2) {
        return Rank::TwoPair;
      }
      
      // do we have a pair?
      if (has2) {
        return Rank::Pair;
      }
      
      return Rank::HighCard;
    }
};

std::ostream& operator<<(std::ostream& os, const Hand& hand) {
  static const std::string rankNames[] = {
    "High Card",
    "Pair",
    "Two Pairs",
    "Three Of A Kind",
    "Straight",
    "Flush",
    "Full House",
    "Four Of A Kind",
    "Straight Flush",
    "Royal Flush"
  };
  std::copy(
    hand.deck.cards.begin(),
    hand.deck.cards.end(),
    std::ostream_iterator<Card>(os, " ")
  );
  os << "(" << rankNames[(int)hand.rank] << ")";
  return os;
}

class Player {
  public:
    Player (std::string const& name) : name(name) {}
    std::string name;
    Hand hand;
};

class Dealer {
  public:
    Dealer () : deck(Deck::Type::Standard) {}
    
    void shuffle() {
      deck.shuffle(14);
    }
    
    void deal(int numCards, std::vector<Player*>& players) {
      int dealt = 0;
      int toDeal = numCards * players.size();
      int deckSize = deck.cards.size();
      
      assert(toDeal < deckSize);
      
      while (dealt < toDeal) {
        for (auto player : players) {
          // std::cout << "dealing to " << player.name << std::endl;
          if (dealt < toDeal) {
            Card& nextCard = deck.cards.at(dealt);
            // std::cout << nextCard << std::endl;
            player->hand.add(
              Card(nextCard.rank, nextCard.suit)
            );
            std::cout << player->name << ": " << player->hand << std::endl;
            // std::cout << player.name << " has " << player.hand.deck.cards.size() << " cards" << std::endl;
          }
          dealt += 1;
        }
      }
      std::cout << "Deal finished" << std::endl;
    }
    
    Deck deck;
};

int main() {
  Player p1("Player 1");
  Player p2("Player 2");
  Dealer dealer;
  
  std::vector<Player*> players;
  players.push_back(&p1);
  players.push_back(&p2);
  
  dealer.shuffle();
  dealer.deal(5, players);
  
  std::cout << p1.name << ":" << p1.hand << std::endl;
  
  std::cout << p2.name << ":" << p2.hand << std::endl;
  
  // Hand hand;
  
  // #define expect(rank){std::cout << hand << std::endl;assert(hand.rank == rank);hand.deck.cards.clear();}
  
  // // royal flush
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::King, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::Queen, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::Jack, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::Ten, Card::Suit::Diamonds));
  // expect(Hand::Rank::RoyalFlush);
  
  // // straight flush
  // hand.add(Card(Card::Rank::Nine, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::King, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::Queen, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::Jack, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::Ten, Card::Suit::Diamonds));
  // expect(Hand::Rank::StraightFlush);
  
  // // four aces
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Spades));
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Clubs));
  // hand.add(Card(Card::Rank::Three, Card::Suit::Diamonds));
  // expect(Hand::Rank::FourOfAKind);
  
  // // full house
  // hand.add(Card(Card::Rank::Two, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Two, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::Two, Card::Suit::Spades));
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Clubs));
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Diamonds));
  // expect(Hand::Rank::FullHouse);
  
  // // flush
  // hand.add(Card(Card::Rank::King, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Four, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Two, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Seven, Card::Suit::Hearts));
  // expect(Hand::Rank::Flush);
  
  // // straight
  // hand.add(Card(Card::Rank::King, Card::Suit::Spades));
  // hand.add(Card(Card::Rank::Queen, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Jack, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Ten, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::Nine, Card::Suit::Hearts));
  // expect(Hand::Rank::Straight);
  
  // // three kings
  // hand.add(Card(Card::Rank::King, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::King, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::King, Card::Suit::Spades));
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Clubs));
  // hand.add(Card(Card::Rank::Three, Card::Suit::Diamonds));
  // expect(Hand::Rank::ThreeOfAKind);
  
  // // 2 pairs
  // hand.add(Card(Card::Rank::Two, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Two, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::King, Card::Suit::Spades));
  // hand.add(Card(Card::Rank::King, Card::Suit::Clubs));
  // hand.add(Card(Card::Rank::Three, Card::Suit::Diamonds));
  // expect(Hand::Rank::TwoPair);
  
  // // pair of 2s
  // hand.add(Card(Card::Rank::Two, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Two, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::King, Card::Suit::Spades));
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Clubs));
  // hand.add(Card(Card::Rank::Three, Card::Suit::Diamonds));
  // expect(Hand::Rank::Pair);
  
  // // high card
  // hand.add(Card(Card::Rank::Two, Card::Suit::Hearts));
  // hand.add(Card(Card::Rank::Seven, Card::Suit::Diamonds));
  // hand.add(Card(Card::Rank::King, Card::Suit::Spades));
  // hand.add(Card(Card::Rank::Ace, Card::Suit::Clubs));
  // hand.add(Card(Card::Rank::Five, Card::Suit::Diamonds));
  // expect(Hand::Rank::Pair);
  
  // Deck deck(Deck::Type::Standard);
  // deck.shuffle(7);
  
  // deck.sort();
  
  // deck.add(Card::Rank::Ace, Card::Suit::Diamonds);
  // deck.add(Card::Rank::King, Card::Suit::Diamonds);
  // deck.add(Card::Rank::Queen, Card::Suit::Diamonds);
  // deck.add(Card::Rank::Jack, Card::Suit::Diamonds);
  
  // deck.log();
  
  // Card ace; ace.rank = Card::Rank::King;
  // Card king; king.rank = Card::Rank::King;
  
  // std::cout << hasHigherRank(ace, king) << std::endl;
  
  // {
  //   Hand a;
  //   Hand b;
    
  //   a.add(Card(Card::Rank::Ace, Card::Suit::Hearts));
  //   a.add(Card(Card::Rank::King, Card::Suit::Hearts));
  //   a.add(Card(Card::Rank::Queen, Card::Suit::Hearts));
  //   a.add(Card(Card::Rank::Jack, Card::Suit::Hearts));
  //   a.add(Card(Card::Rank::Ten, Card::Suit::Hearts));
    
  //   b.add(Card(Card::Rank::Ace, Card::Suit::Hearts));
  //   b.add(Card(Card::Rank::King, Card::Suit::Clubs));
  //   b.add(Card(Card::Rank::Queen, Card::Suit::Spades));
  //   b.add(Card(Card::Rank::Jack, Card::Suit::Diamonds));
  //   b.add(Card(Card::Rank::Ten, Card::Suit::Hearts));
    
  //   std::cout << a << " beats " << b << " ? " << (a > b ? "Yes" : "No") << std::endl;
  // }
  
  return 0;
}