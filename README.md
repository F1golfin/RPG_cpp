## Compilation

Commande conseillee sous macOS/Linux :

```bash
g++ -std=c++17 main.cpp Controller/*.cpp Model/*.cpp Model/Player/*.cpp Model/Monster/*.cpp -I. -o rpg_cpp
```

Commande possible sous Windows :

```bash
g++ -std=c++17 main.cpp Controller/*.cpp Model/*.cpp Model/Player/*.cpp Model/Monster/*.cpp -I. -o rpg_app.exe
```

## Lancement

```bash
./rpg_cpp
```

Au lancement, on demande le nom du joueur, puis le jeu charge les fichiers `Data/items.csv` et `Data/monsters.csv`.

## Fonctionnalites principales

- Menu principal avec acces au combat, au bestiaire complet, a l'historique des combats, aux statistiques du joueur et a l'inventaire.
- Combats au tour par tour avec les actions `FIGHT`, `ACT`, `ITEM` et `MERCY`.
- Systeme de Mercy : on peut epargner un monstre seulement quand sa jauge Mercy atteint son objectif.
- Systeme ACT avec actions positives et negatives.
- Effets secondaires sur certaines actions ACT :
  - `OBSERVE` et `REASON` diminuent l'attaque du monstre.
  - `OFFER_SNACK` et `APOLOGIZE` soignent legerement le joueur.
  - `INSULT` et `TAUNT` rendent le monstre plus dangereux.
  - `RESPECT` et `CHALLENGE` diminuent la defense du monstre.
- Inventaire avec items charges depuis `items.csv`.
- Le joueur commence avec seulement 2 items de depart, principalement des petits soins.
- Les monstres tues peuvent dropper des items selon leur categorie :
  - `NORMAL` : chance de drop faible, surtout des petits soins.
  - `MINIBOSS` : chance de drop moyenne, soins ou petits buffs.
  - `BOSS` : chance de drop elevee, tous les items possibles.
- Plusieurs types d'items :
  - `HEAL` rend des HP au joueur.
  - `BUFF_ATTACK` augmente durablement l'attaque du joueur.
  - `BUFF_DEFENSE` reduit durablement les degats recus.
- Bestiaire complet avec statistiques des monstres.
- Historique des combats gagnes avec statut `Killed` ou `Spared`.
- Rechargement des CSV depuis le menu principal.
- Progression par difficulte :
  - monstres `NORMAL` au debut,
  - `MINIBOSS` apres plusieurs victoires,
  - `BOSS` en fin de partie.
- Condition de victoire apres 10 combats gagnes.
- Fins multiples :
  - `Genocide` si toutes les victoires sont obtenues en tuant,
  - `True Pacifist` si les 10 victoires sont obtenues uniquement avec Mercy,
  - `Pacifist` si toutes les victoires disponibles sont obtenues avec Mercy,
  - `Neutral` si le joueur melange kills et spares,
  - `Defeat` si le joueur meurt.
- Resume final avec nombre de victoires, kills et spares.
- Phrases d'introduction propres a chaque monstre, chargees depuis `monsters.csv`.

## Donnees CSV

### `Data/items.csv`

Le fichier contient la reserve totale d'items disponibles pendant la partie :

```csv
nom;type;valeur;quantite
Estus Flask;HEAL;20;3
Ember;BUFF_ATTACK;2;2
Sunlit Balm;BUFF_DEFENSE;2;2
```

- `nom` : nom affiche dans l'inventaire.
- `type` : type de l'objet : `HEAL`, `BUFF_ATTACK` ou `BUFF_DEFENSE`.
- `valeur` : HP rendus pour `HEAL`, bonus d'attaque pour `BUFF_ATTACK`, reduction de degats pour `BUFF_DEFENSE`.
- `quantite` : nombre d'exemplaires disponibles dans la reserve globale.

Au lancement, on donne seulement deux items au joueur. Le reste du stock peut etre obtenu en tuant des monstres. Quand on utilise un item, le jeu affiche l'effet obtenu : HP rendus, bonus d'attaque gagne ou bonus de defense gagne. Les buffs restent actifs pendant la partie, mais ils sont equilibres par la rarete des drops et par les quantites disponibles dans le CSV. Les statistiques affichent les bonus `ATK` et `DEF`.

### `Data/monsters.csv`

Le fichier contient tous les monstres possibles :

```csv
categorie;nom;hp;atk;def;mercyGoal;act1;act2;act3;act4;intro
NORMAL;Hollow Peasant;28;6;1;100;OBSERVE;COMPLIMENT;-;-;A frightened peasant blocks the path.
```

- `categorie` : `NORMAL`, `MINIBOSS` ou `BOSS`.
- `nom` : nom du monstre.
- `hp`, `atk`, `def` : statistiques de combat.
- `mercyGoal` : valeur a atteindre pour pouvoir utiliser Mercy.
- `act1` a `act4` : identifiants des actions ACT disponibles.
- `intro` : phrase affichee au debut du combat.

## Architecture POO

Le projet est organise en deux grandes parties :

- `Model/` contient les classes metier : `Entity`, `Player`, `Monster`, `Item`, `InventorySlot`, `ActAction`, `BestiaryEntry`.
- `Controller/` contient la boucle de jeu et l'affichage console : `Game` et `Renderer`.

Concepts POO utilises :

- Encapsulation : les attributs importants sont prives ou proteges, et on passe par des methodes pour les modifier.
- Heritage : `Player` et `Monster` heritent de la classe abstraite `Entity`.
- Classe abstraite : `Entity` contient la methode virtuelle pure `attack`.
- Polymorphisme : les monstres derives redefinissent `attack`.
- Composition : `Game` contient le joueur, le catalogue d'actions, le bestiaire et le pool de monstres ; le joueur contient son inventaire.

## Polymorphisme des monstres

On utilise maintenant trois classes derivees de `Monster` :

- `NormalMonster` : attaque classique.
- `MiniBoss` : attaque plus fort si sa Mercy est encore basse.
- `BossMonster` : declenche une attaque speciale tous les trois tours ou quand ses HP sont bas.

Cela permet d'avoir un comportement different selon le type reel du monstre, pas seulement selon une condition sur une enum.

## Pourquoi on utilise `clone()`

Le jeu garde un pool de monstres charges depuis le CSV. Ces monstres servent de modeles : on ne veut pas modifier leurs HP ou leur Mercy directement, sinon le monstre resterait affaibli lors des combats suivants.

Avant, on pouvait copier un monstre avec :

```cpp
return new Monster(*possibleMonsters[index]);
```

Le probleme est que cette copie recrée toujours un objet de type `Monster`. Si le monstre choisi etait un `MiniBoss` ou un `BossMonster`, on perdait son vrai type derive. Son attaque redevenait donc l'attaque de base de `Monster`, ce qui cassait le polymorphisme.

Pour eviter cela, chaque classe implemente une methode virtuelle `clone()` :

```cpp
virtual Monster* clone() const;
```

Puis chaque classe derivee retourne une copie de son propre type :

```cpp
Monster* BossMonster::clone() const
{
    return new BossMonster(*this);
}
```

Dans `Game::selectRandomMonster`, on appelle :

```cpp
return possibleMonsters[index]->clone();
```

Ainsi, on obtient une copie independante pour le combat, tout en conservant le type reel du monstre. C'est ce qui permet a `NormalMonster`, `MiniBoss` et `BossMonster` d'avoir chacun leur propre comportement d'attaque.

## Routes de jeu

- Route genocide : utiliser principalement `FIGHT` et tuer les monstres.
- Route pacifiste : utiliser `ACT` pour remplir la jauge Mercy, puis `MERCY`.
- Route neutre : melanger kills et spares.

Le jeu se termine quand le joueur atteint 10 victoires ou quand il n'a plus de HP.
