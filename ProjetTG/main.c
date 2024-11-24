#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <math.h>

#define MAX_NODES 6
#define MAX_ARCS 9

// Structure pour représenter un arc
typedef struct {
    int id;            // Identifiant de l'arc
    int source;        // Sommet source
    int destination;   // Sommet destination
    float poids;       // Poids de l'arc
} Arcs;

// Structure pour représenter un réseau trophique
typedef struct {
    int id;
    char nom[100];
    char type[100];
    double population;  // Population de l'espèce
    double r;           // Taux de croissance
    double K;           // Capacité de portage
    double N;           // Population actuelle
    double biomasse;    // Biomasse en kg
    double flux_energie; // Flux énergétique en Watts
    double flux_nutriments; // Flux de nutriments supplémentaire
} Noeud;

// Fonction pour afficher les informations d'un sommet
void afficher_sommet(Noeud *noeud) {
    printf("Sommet %d: %s (%s) - Population: %.2f\n", noeud->id, noeud->nom, noeud->type, noeud->N);
}// Fonction pour afficher les arcs
void afficher_arc(Arcs *arc) {
    printf("Arc: %d -> %d avec poids %.2f\n", arc->source, arc->destination, arc->poids);
}

// Fonction pour afficher les successeurs et prédécesseurs d'un sommet
void afficher_adjacence(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("\nSommet %d: %s\n", noeuds[i].id, noeuds[i].nom);
        printf("Successeurs: ");
        for (int j = 0; j < nombre_arcs; j++) {
            if (arcs[j].source == i) {
                printf("%d ", arcs[j].destination);
            }
        }

        printf("\nPrédécesseurs: ");
        for (int j = 0; j < nombre_arcs; j++) {
            if (arcs[j].destination == i) {
                printf("%d ", arcs[j].source);
            }
        }
        printf("\n");
    }
}

// Fonction pour afficher les espèces avec plusieurs prédateurs
void afficher_especes_avec_multiple_predateurs(Arcs *arcs, int nombre_arcs, Noeud *noeuds, int nombre_noeuds) {
    printf("\nEspèces avec plusieurs prédateurs :\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        int count = 0;
        for (int j = 0; j < nombre_arcs; j++) {
            if (arcs[j].destination == i) {
                count++;
            }
        }
        if (count > 1) {
            printf("%s\n", noeuds[i].nom);
        }
    }
}

// Fonction pour afficher les espèces avec plusieurs sources d'alimentation
void afficher_especes_avec_multiple_sources(Arcs *arcs, int nombre_arcs, Noeud *noeuds, int nombre_noeuds) {
    printf("\nEspèces avec plusieurs sources d'alimentation :\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        int count = 0;
        for (int j = 0; j < nombre_arcs; j++) {
            if (arcs[j].source == i) {
                count++;
            }
        }
        if (count > 1) {
            printf("%s\n", noeuds[i].nom);
        }
    }
}

// Fonction pour afficher les espèces les plus connectées
void afficher_especes_plus_connectees(Arcs *arcs, int nombre_arcs, Noeud *noeuds, int nombre_noeuds) {
    printf("\nEspèces les plus connectées :\n");
    int *degres = (int *)malloc(nombre_noeuds * sizeof(int));
    for (int i = 0; i < nombre_noeuds; i++) {
        degres[i] = 0;
    }

    for (int i = 0; i < nombre_arcs; i++) {
        degres[arcs[i].source]++;
        degres[arcs[i].destination]++;
    }

    int max_degre = 0;
    for (int i = 0; i < nombre_noeuds; i++) {
        if (degres[i] > max_degre) {
            max_degre = degres[i];
        }
    }

    for (int i = 0; i < nombre_noeuds; i++) {
        if (degres[i] == max_degre) {
            printf("%s\n", noeuds[i].nom);
        }
    }

    free(degres);
}

// Fonction pour afficher les espèces avec des prédateurs et des proies
void afficher_especes_avec_predateurs_et_proies(Arcs *arcs, int nombre_arcs, Noeud *noeuds, int nombre_noeuds) {
    printf("\nEspèces avec des prédateurs et des proies :\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        int a_predateur = 0, a_proie = 0;
        for (int j = 0; j < nombre_arcs; j++) {
            if (arcs[j].destination == i) {
                a_predateur = 1;
            }
            if (arcs[j].source == i) {
                a_proie = 1;
            }
        }
        if (a_predateur && a_proie) {
            printf("%s\n", noeuds[i].nom);
        }
    }
}

// Fonction pour afficher les nœuds isolés
void afficher_noeuds_isoles(Arcs *arcs, int nombre_arcs, Noeud *noeuds, int nombre_noeuds) {
    printf("\nNoeuds isolés :");
    int noeuds_isoles_count = 0;
    for (int i = 0; i < nombre_noeuds; i++) {
        int a_predateur = 0, a_proie = 0;
        for (int j = 0; j < nombre_arcs; j++) {
            if (arcs[j].destination == i) {
                a_predateur = 1;
            }
            if (arcs[j].source == i) {
                a_proie = 1;
            }
        }
        if (!a_predateur && !a_proie) {
            printf("%s\n", noeuds[i].nom);
            noeuds_isoles_count++;
        }
    }
    if (noeuds_isoles_count == 0) {
        printf(" 0");
    }
}

// Fonction pour calculer et afficher la complexité du réseau
void afficher_complexite(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    // 1. Nombre d'espèces
    printf("\nNombre d'espèces : %d\n", nombre_noeuds);

    // 2. Hauteur trophique
    int niveaux[nombre_noeuds];
    for (int i = 0; i < nombre_noeuds; i++) {
        niveaux[i] = -1; // Initialisation
    }
    niveaux[0] = 0; // Les producteurs (ID 0) sont au niveau 0

    // Calcul de la hauteur trophique
    for (int i = 0; i < nombre_arcs; i++) {
        int source = arcs[i].source;
        int destination = arcs[i].destination;
        if (niveaux[source] != -1) {
            if (niveaux[destination] < niveaux[source] + 1) {
                niveaux[destination] = niveaux[source] + 1;
            }
        }
    }

    int hauteur_trophique = 0;
    for (int i = 0; i < nombre_noeuds; i++) {
        if (niveaux[i] > hauteur_trophique) {
            hauteur_trophique = niveaux[i];
        }
    }
    printf("Hauteur trophique : %d niveaux\n", hauteur_trophique + 1);

    // 3. Densité de liaison
    float densite = (float)nombre_arcs / (nombre_noeuds * (nombre_noeuds - 1));
    printf("Densité de liaison : %.2f\n\n", densite);

    // 4. Distribution des degrés
    int degres[nombre_noeuds];
    for (int i = 0; i < nombre_noeuds; i++) {
        degres[i] = 0;
    }

    for (int i = 0; i < nombre_arcs; i++) {
        degres[arcs[i].source]++;
        degres[arcs[i].destination]++;
    }

    printf("Distribution des degrés :\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("Sommet %d : Degré %d\n", i, degres[i]);
    }
}

// Fonction DFS pour la vérification de la connexité
void dfs(int sommet, int *visited, int adj[MAX_NODES][MAX_NODES], int nombre_noeuds) {
    visited[sommet] = 1;
    for (int i = 0; i < nombre_noeuds; i++) {
        if (adj[sommet][i] && !visited[i]) {
            dfs(i, visited, adj, nombre_noeuds);
        }
    }
}

// Fonction pour vérifier la connexité faible
int verifier_connexite_faible(Arcs *arcs, int nombre_arcs, int nombre_noeuds) {
    // Matrice d'adjacence non dirigée
    int adj[MAX_NODES][MAX_NODES] = {0};

    // Remplissage de la matrice d'adjacence
    for (int i = 0; i < nombre_arcs; i++) {
        adj[arcs[i].source][arcs[i].destination] = 1;
        adj[arcs[i].destination][arcs[i].source] = 1; // Ignorer la direction
    }

    // Tableau pour marquer les sommets visités
    int visited[MAX_NODES] = {0};

    // Lancer DFS depuis le sommet 0
    dfs(0, visited, adj, nombre_noeuds);

    // Vérifier si tous les sommets ont été visités
    for (int i = 0; i < nombre_noeuds; i++) {
        if (!visited[i]) {
            return 0; // Le graphe n'est pas connexe
        }
    }
    return 1; // Le graphe est connexe
}

// Charger le réseau et initialiser noeud_count et arc_count
void charger_et_afficher_reseau(const char *nom_fichier, Noeud *noeuds, Arcs *arcs, int *noeud_count, int *arc_count) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strncmp(ligne, "# Nœuds", 7) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (sscanf(ligne, "%d %s %s %lf %lf %lf", &noeuds[*noeud_count].id, noeuds[*noeud_count].nom, noeuds[*noeud_count].type, &noeuds[*noeud_count].r, &noeuds[*noeud_count].K, &noeuds[*noeud_count].N) == 6) {
                    (*noeud_count)++;
                }
            }
        }
        if (strncmp(ligne, "# Arcs", 6) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (sscanf(ligne, "%d %d %d %f", &arcs[*arc_count].id, &arcs[*arc_count].source, &arcs[*arc_count].destination, &arcs[*arc_count].poids) == 4) {
                    (*arc_count)++;
                }
            }
        }
    }

    fclose(fichier);

    // Affichage des informations
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < *noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }

    printf("\n=== Affichage des arcs ===\n");
    for (int i = 0; i < *arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    printf("\n=== Affichage des successeurs et prédécesseurs ===");
    afficher_adjacence(noeuds, arcs, *noeud_count, *arc_count);
}
void afficher_niveaux_trophiques(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int espece_id) {
    int niveaux[MAX_NODES];
    for (int i = 0; i < nombre_noeuds; i++) {
        niveaux[i] = -1; // Initialisation
    }
    niveaux[0] = 0; // Producteur primaire au niveau 0

    for (int i = 0; i < nombre_arcs; i++) {
        int source = arcs[i].source;
        int destination = arcs[i].destination;
        if (niveaux[source] != -1 && niveaux[destination] < niveaux[source] + 1) {
            niveaux[destination] = niveaux[source] + 1;
        }
    }

    if (niveaux[espece_id] != -1) {
        printf("\nNiveau trophique de %s : %d\n", noeuds[espece_id].nom, niveaux[espece_id]);
    } else {
        printf("\nNiveau trophique non calculable pour %s.\n", noeuds[espece_id].nom);
    }
}

void afficher_graphe_ascendants_recursif(Noeud *noeuds, Arcs *arcs, int nombre_arcs, int espece_id, int niveau) {
    int found = 0;
    for (int i = 0; i < nombre_arcs; i++) {
        if (arcs[i].destination == espece_id) {
            found = 1;
            for (int j = 0; j < niveau; j++) {
                printf("  "); // Indentation pour montrer le niveau
            }
            printf("%s <- %s\n", noeuds[espece_id].nom, noeuds[arcs[i].source].nom);
            afficher_graphe_ascendants_recursif(noeuds, arcs, nombre_arcs, arcs[i].source, niveau + 1);
        }
    }

    if (!found && niveau == 0) {
        printf("Aucune chaîne alimentaire trouvée pour %s.\n", noeuds[espece_id].nom);
    }
}

void afficher_graphe_ascendants(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int espece_id) {
    printf("\nChaînes alimentaires complètes dont dépend %s :\n", noeuds[espece_id].nom);
    afficher_graphe_ascendants_recursif(noeuds, arcs, nombre_arcs, espece_id, 0);
}

// Fonction pour calculer les degrés (entrée et sortie)
void calculer_degres(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    int in_degree[MAX_NODES] = {0};
    int out_degree[MAX_NODES] = {0};

    for (int i = 0; i < nombre_arcs; i++) {
        out_degree[arcs[i].source]++;
        in_degree[arcs[i].destination]++;
    }

    printf("\n=== Degrés des espèces ===\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("%s: in-degree = %d, out-degree = %d\n", noeuds[i].nom, in_degree[i], out_degree[i]);
    }
}

// Fonction pour calculer la centralité d'intermédiarité
void calculer_centralite_intermediarite(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    float centralite[MAX_NODES] = {0};
    int adj[MAX_NODES][MAX_NODES] = {0};

    // Construire la matrice d'adjacence
    for (int i = 0; i < nombre_arcs; i++) {
        adj[arcs[i].source][arcs[i].destination] = 1;
    }

    // Calcul de la centralité d'intermédiarité (simplifiée)
    for (int k = 0; k < nombre_noeuds; k++) {
        for (int i = 0; i < nombre_noeuds; i++) {
            for (int j = 0; j < nombre_noeuds; j++) {
                if (i != j && i != k && j != k && adj[i][k] && adj[k][j]) {
                    centralite[k]++;
                }
            }
        }
    }

    printf("\n=== Centralité d'intermédiarité ===\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("%s: centralité = %.2f\n", noeuds[i].nom, centralite[i]);
    }
}

// Fonction pour simuler la disparition d'une espèce
void simuler_disparition(Noeud *noeuds, Arcs *arcs, int *nombre_arcs, int espece_id) {
    printf("\n=== Simulation de la disparition de %s ===\n", noeuds[espece_id].nom);

    // Afficher les arcs liés à l'espèce disparue
    printf("\nArcs liés à %s avant la disparition :\n", noeuds[espece_id].nom);
    for (int i = 0; i < *nombre_arcs; i++) {
        if (arcs[i].source == espece_id || arcs[i].destination == espece_id) {
            afficher_arc(&arcs[i]);
        }
    }

    // Suppression des arcs liés à l'espèce disparue
    int nouveaux_arcs[MAX_ARCS];
    int compteur = 0;
    for (int i = 0; i < *nombre_arcs; i++) {
        if (arcs[i].source != espece_id && arcs[i].destination != espece_id) {
            arcs[compteur++] = arcs[i]; // Garder l'arc
        }
    }
    *nombre_arcs = compteur; // Mettre à jour le nombre d'arcs après suppression

    // Afficher les arcs restants après suppression
    printf("\nArcs après la disparition de %s :\n", noeuds[espece_id].nom);
    for (int i = 0; i < *nombre_arcs; i++) {
        afficher_arc(&arcs[i]);
    }
}

// Function to get the terminal width on Windows
int get_terminal_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    return columns;
}

// Function to get the terminal height on Windows
int get_terminal_height() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return rows;
}

// Function to center and print text
void print_centered(const char *text) {
    int terminal_width = get_terminal_width();
    int text_length = strlen(text);
    int padding = (terminal_width - text_length) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

// Function to center and print text exactly in the middle of the console
void text_centered(const char *text) {
    int terminal_width = get_terminal_width();
    int terminal_height = get_terminal_height();
    int text_length = strlen(text);
    int padding_left = (terminal_width - text_length) / 2;
    int padding_top = terminal_height / 2;

    // Move the cursor to the middle of the console
    COORD coord;
    coord.X = padding_left;
    coord.Y = padding_top;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // Print the centered text
    printf("%s\n", text);
}

// Function to center the cursor on the screen
void center_cursor() {
    int terminal_width = get_terminal_width();
    int terminal_height = get_terminal_height();
    COORD coord;
    coord.X = terminal_width / 2 - 7;
    coord.Y = (terminal_height / 2) + 5; // Move down by 5 lines
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void afficher_loading(const char *message) {
    system("cls"); // Clear the screen
    text_centered(message);
    COORD coord;
    coord.X = get_terminal_width() / 2 + strlen(message) / 2;
    coord.Y = get_terminal_height() / 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    for (int i = 0; i < 3; i++) {
        Sleep(500); // Wait for 1 second
        printf(".");
        fflush(stdout); // Ensure the dot is printed immediately
    }
    Sleep(1000); // Wait for 1 second before proceeding
    system("cls"); // Clear the screen
}

// Fonction pour mettre à jour la population basée sur la croissance logistique
void mettre_a_jour_population(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int iteration) {
    for (int i = 0; i < nombre_noeuds; i++) {
        // Initialisation correcte des valeurs de biomasse et flux énergétique
        if (noeuds[i].biomasse < 0) {
            noeuds[i].biomasse = 0;
        }
        if (noeuds[i].flux_energie < 0 || noeuds[i].flux_energie > 1000.0) {
            noeuds[i].flux_energie = 0; // Set to 0 if the value is unrealistic
        }

        // Calcul de la croissance logistique avec variation saisonnière et aléatoire
        double seasonal_factor = 1.0 - 0.1 * sin(iteration * 0.1); // Variation saisonnière (réduction)
        double random_factor = 1.0 - ((rand() % 21) / 100.0); // Variation aléatoire entre 0% et -20%
        double croissance = noeuds[i].r * noeuds[i].N * (1 - noeuds[i].N / noeuds[i].K) * seasonal_factor * random_factor;

        // Prendre en compte les interactions des arcs : prédation et ressources
        for (int j = 0; j < nombre_arcs; j++) {
            if (arcs[j].source == noeuds[i].id) {
                int dest = arcs[j].destination;
                double poids = arcs[j].poids;
                croissance -= poids * noeuds[dest].N * 0.1; // Impact de la prédation
            }
        }

        // Facteur humain impactant
        if (strcmp(noeuds[i].type, "Habitat") == 0) {
            croissance *= 0.5; // Réduire la croissance de 50% pour les habitats affectés par l'activité humaine
        }

        // Facteur de mortalité basé sur la densité de population
        double mortality_factor = 1.0 + 0.05 * (noeuds[i].N / noeuds[i].K); // Augmente avec la densité de population
        croissance /= mortality_factor; // Appliquer le facteur de mortalité

        // Mise à jour de la population après les interactions
        noeuds[i].N += croissance;

        // Limite la population à la capacité de portage
        if (noeuds[i].N > noeuds[i].K) {
            noeuds[i].N = noeuds[i].K;
        } else if (noeuds[i].N < 0) {
            noeuds[i].N = 0;
        }

        // Homogénéisation des biomasses et flux énergétiques
        noeuds[i].biomasse = noeuds[i].N * 0.1; // Exemple : 0.1 kg par unité de population
        noeuds[i].flux_energie = noeuds[i].N * 0.05; // Exemple : 0.05 W par unité de population

        // Correction des valeurs de flux énergétique pour éviter des valeurs irréalistes
        if (noeuds[i].flux_energie > 1000.0) {
            noeuds[i].flux_energie = 1000.0;
        }
    }
}

int animation_en_cours = 0; // 0: pause, 1: en cours
void lancer_animation() {
    animation_en_cours = 1;
}
void pauser_animation() {
    animation_en_cours = 0;
}

void modifier_valeurs(Noeud *noeuds, int nombre_noeuds) {
    int id;
    double nouvelle_valeur;
    printf("Entrez l'ID du sommet à modifier : ");
    scanf("%d", &id);
    if (id < 0 || id >= nombre_noeuds) {
        printf("ID invalide.\n");
        return;
    }
    printf("Entrez la nouvelle valeur de la population : ");
    scanf("%lf", &nouvelle_valeur);
    noeuds[id].N = nouvelle_valeur;
    printf("Population du sommet %d mise à jour à %.2f\n", id, noeuds[id].N);
}

void neutraliser_sommet(Noeud *noeuds, int nombre_noeuds) {
    int id;
    printf("Entrez l'ID du sommet à neutraliser : ");
    scanf("%d", &id);
    if (id < 0 || id >= nombre_noeuds) {
        printf("ID invalide.\n");
        return;
    }
    noeuds[id].N = 0;
    printf("Sommet %d neutralisé.\n", id);
}

// Fonction pour simuler l'évolution de la population au fil du temps
void simuler_evolution_population(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    int t = 0;
    while (1) {
        mettre_a_jour_population(noeuds, arcs, nombre_noeuds, nombre_arcs, t);

        // Affichage de l'état du réseau après chaque itération
        printf("Iteration %d:\n", t + 1);
        for (int i = 0; i < nombre_noeuds; i++) {
            printf("Sommet %d: %s - Population: %.2f\n", noeuds[i].id, noeuds[i].nom, noeuds[i].N);
        }
        printf("\n");

        printf("Animation en pause. Appuyez sur 'c' pour continuer, 'm' pour modifier les valeurs, 'n' pour neutraliser un sommet, 'q' pour quitter.\n");
        char choix = _getch();
        if (choix == 'c') {
            t++; // Avancer l'itération
        } else if (choix == 'm') {
            modifier_valeurs(noeuds, nombre_noeuds);
        } else if (choix == 'n') {
            neutraliser_sommet(noeuds, nombre_noeuds);
        } else if (choix == 'q') {
            break;
        }
    }
}
// Fonction pour afficher le réseau, les nœuds et les arcs
void afficher_reseau(Noeud *noeuds, Arcs *arcs, int noeud_count, int arc_count) {
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }

    printf("\n=== Affichage des arcs ===\n");
    for (int i = 0; i < arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    printf("\n=== Affichage des successeurs et prédécesseurs ===");
    afficher_adjacence(noeuds, arcs, noeud_count, arc_count);
}

// Fonction pour charger le réseau depuis un fichier
void charger_reseau(const char *nom_fichier, Noeud *noeuds, Arcs *arcs, int *noeud_count, int *arc_count) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }

    char ligne[256];
    *noeud_count = 0;
    *arc_count = 0;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strncmp(ligne, "# Nœuds", 7) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (*noeud_count >= MAX_NODES) {
                    fclose(fichier);
                    return;
                }
                if (sscanf(ligne, "%d %s %s %lf %lf %lf", &noeuds[*noeud_count].id, noeuds[*noeud_count].nom, noeuds[*noeud_count].type, &noeuds[*noeud_count].r, &noeuds[*noeud_count].K, &noeuds[*noeud_count].N) == 6) {
                    (*noeud_count)++;
                }
            }
        }
        if (strncmp(ligne, "# Arcs", 6) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (*arc_count >= MAX_ARCS) {
                    fclose(fichier);
                    return;
                }
                if (sscanf(ligne, "%d %d %d %f", &arcs[*arc_count].id, &arcs[*arc_count].source, &arcs[*arc_count].destination, &arcs[*arc_count].poids) == 4) {
                    (*arc_count)++;
                }
            }
        }
    }

    fclose(fichier);
}

void runge_kutta_2(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, double dt) {
    for (int i = 0; i < nombre_noeuds; i++) {
        double k1 = noeuds[i].r * noeuds[i].N * (1 - noeuds[i].N / noeuds[i].K);
        double k2 = noeuds[i].r * (noeuds[i].N + dt * k1 / 2) * (1 - (noeuds[i].N + dt * k1 / 2) / noeuds[i].K);
        noeuds[i].N += dt * k2;
        if (noeuds[i].N > noeuds[i].K) {
            noeuds[i].N = noeuds[i].K;
        } else if (noeuds[i].N < 0) {
            noeuds[i].N = 0;
        }
    }
}

void appliquer_facteurs_anthropiques(Noeud *noeuds, int nombre_noeuds) {
    for (int i = 0; i < nombre_noeuds; i++) {
        if (strcmp(noeuds[i].type, "Habitat") == 0) {
            noeuds[i].K *= 0.5;  // Exemple : réduire la capacité de portage de 50%
        }
    }
}

void plot_population(Noeud *noeuds, int nombre_noeuds, int iteration) {
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    fprintf(gnuplot, "set terminal png\n");
    fprintf(gnuplot, "set output 'population_%d.png'\n", iteration);
    fprintf(gnuplot, "plot '-' using 1:2 with lines title 'Population'\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        fprintf(gnuplot, "%d %.2f\n", i, noeuds[i].N);
    }
    fprintf(gnuplot, "e\n");
    pclose(gnuplot);
}

void afficher_rapport(Noeud *noeuds, Arcs *arcs, int noeud_count, int arc_count) {
    system("cls");
    printf("\n=== Rapport de Synthèse ===\n");

    // Affichage des sommets et arcs
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }
    for (int i = 0; i < arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    // Affichage des successeurs et prédécesseurs
    printf("\n=== Affichage des successeurs et prédécesseurs ===\n");
    for (int i = 0; i < noeud_count; i++) {
        printf("Sommet %d: %s\nSuccesseurs:", noeuds[i].id, noeuds[i].nom);
        for (int j = 0; j < arc_count; j++) {
            if (arcs[j].source == noeuds[i].id) {
                printf(" %d", arcs[j].destination);
            }
        }
        printf("\nPrédécesseurs:");
        for (int j = 0; j < arc_count; j++) {
            if (arcs[j].destination == noeuds[i].id) {
                printf(" %d", arcs[j].source);
            }
        }
        printf("\n");
    }

    // Espèces avec plusieurs prédateurs
    afficher_especes_avec_multiple_predateurs(arcs, arc_count, noeuds, noeud_count);

    // Espèces avec plusieurs sources d'alimentation
    afficher_especes_avec_multiple_sources(arcs, arc_count, noeuds, noeud_count);

    // Espèces les plus connectées
    afficher_especes_plus_connectees(arcs, arc_count, noeuds, noeud_count);

    // Espèces avec des prédateurs et des proies
    afficher_especes_avec_predateurs_et_proies(arcs, arc_count, noeuds, noeud_count);

    // Noeuds isolés
    afficher_noeuds_isoles(arcs, arc_count, noeuds, noeud_count);

    // Complexité du réseau
    afficher_complexite(noeuds, arcs, noeud_count, arc_count);

    // Vérification de la connexité
    int connexe = verifier_connexite_faible(arcs, arc_count, noeud_count);
    printf("\nLe graphe est %sconnexe.\n", connexe ? "" : "non ");

    // Résultats de la simulation
    printf("\n=== Résultats de la Simulation ===\n");
    for (int i = 0; i < noeud_count; i++) {
        printf("Sommet %d: %s (%s) - Population: %.2f\n", noeuds[i].id, noeuds[i].nom, noeuds[i].type, noeuds[i].N);
    }
}

int main() {
    Noeud noeuds[MAX_NODES];
    Arcs arcs[MAX_ARCS];
    int noeud_count = 0, arc_count = 0;  // Initialisation de noeud_count et arc_count

    char choix[256];
    char espece_nom[100];  // Déclare une seule fois l'input pour l'espèce
    int espece_id = -1;     // Déclare une seule fois l'ID de l'espèce
    char nomfichier[256];   // Déclare la variable nomfichier

    while (1) {
        system("cls"); // Clear the screen
        printf("\t\t\t\t\t _______              _     _\n");
        printf("\t\t\t\t\t|__   __|            | |   (_)\n");
        printf("\t\t\t\t\t   | |_ __ ___  _ __ | |__  _  ___\n");
        printf("\t\t\t\t\t   | | '__/ _ \\| '_ \\| '_ \\| |/ __|\n");
        printf("\t\t\t\t\t   | | | | (_) | |_) | | | | | (__\n");
        printf("\t\t\t\t\t _ |_|_|  \\___/| .__/|_| |_|_|\\___|_\n");
        printf("\t\t\t\t\t| \\ | |    | | | |                | |\n");
        printf("\t\t\t\t\t|  \\| | ___| |_|_|     _____  _ __| | _____\n");
        printf("\t\t\t\t\t| . ` |/ _ \\ __\\ \\ /\\ / / _ \\| '__| |/ / __|\n");
        printf("\t\t\t\t\t| |\\  |  __/ |_ \\ V  V / (_) | |  |   <\\__ \\\n");
        printf("\t\t\t\t\t|_| \\_|\\___|\\__| \\_/\\_/ \\___/|_|  |_|\\_\\___/\n\n");

        print_centered("Choisir un réseau trophique:\n\n");
        print_centered("1. Foret temperee");
        print_centered("2. Ocean Arctique");
        print_centered("3. Savane Africaine");
        print_centered("4. Quitter");
        center_cursor();
        fgets(choix, sizeof(choix), stdin);
        choix[strcspn(choix, "\n")] = '\0'; // Retirer le caractère de nouvelle ligne

        if (strcmp(choix, "Foret temperee") == 0) {
            afficher_loading("Loading");
            strcpy(nomfichier, "graphe1.txt");
        } else if (strcmp(choix, "Ocean Arctique") == 0) {
            afficher_loading("Loading");
            strcpy(nomfichier, "graphe2.txt");
        } else if (strcmp(choix, "Savane Africaine") == 0) {
            afficher_loading("Loading");
            strcpy(nomfichier, "graphe3.txt");
        } else if (strcmp(choix, "Quitter") == 0) {
            break;
        } else {
            afficher_loading("Erreur");
            continue;
        }

        charger_et_afficher_reseau(nomfichier, noeuds, arcs, &noeud_count, &arc_count);

        // Demander à l'utilisateur de sélectionner une espèce par son nom
        printf("\nEntrez le nom de l'espèce pour afficher les chaînes alimentaires dont elle dépend : ");
        scanf("%s", espece_nom);

        // Trouver l'ID correspondant au nom entré
        espece_id = -1;
        for (int i = 0; i < noeud_count; i++) {
            if (strcmp(noeuds[i].nom, espece_nom) == 0) {
                espece_id = noeuds[i].id;
                break;
            }
        }

        if (espece_id != -1) {
            afficher_graphe_ascendants(noeuds, arcs, noeud_count, arc_count, espece_id);
            afficher_niveaux_trophiques(noeuds, arcs, noeud_count, arc_count, espece_id);
        } else {
            printf("Espèce \"%s\" introuvable dans le réseau.\n", espece_nom);
        }

        // Demander à l'utilisateur de simuler la disparition d'une espèce
        printf("\nEntrez le nom de l'espèce à simuler pour disparition : ");
        scanf("%s", espece_nom);

        // Trouver l'ID correspondant à l'espèce
        espece_id = -1;
        for (int i = 0; i < noeud_count; i++) {
            if (strcmp(noeuds[i].nom, espece_nom) == 0) {
                espece_id = i;
                break;
            }
        }

        if (espece_id != -1) {
            simuler_disparition(noeuds, arcs, &arc_count, espece_id);
            calculer_degres(noeuds, arcs, noeud_count, arc_count); // Recalcul des degrés
            calculer_centralite_intermediarite(noeuds, arcs, noeud_count, arc_count); // Recalcul de la centralité
        } else {
            printf("Espèce \"%s\" introuvable.\n", espece_nom);
        }
        charger_reseau(nomfichier, noeuds, arcs, &noeud_count, &arc_count);
        simuler_evolution_population(noeuds, arcs, noeud_count, arc_count);
        system("cls");
        afficher_rapport(noeuds, arcs, noeud_count, arc_count);

        while (_getch() != 27); // 27 is the ASCII code for Esc
    }

    return 0;
}

//DNEZBCNHIZDNCIZNDCINZIHDHCNIDNCHIZDNVIBZNCVINZUCVNZIUNCIUZNVCBNRZIV

  // 3. Densité de liaison
    float densite = (float)nombre_arcs / (nombre_noeuds * (nombre_noeuds - 1));
    printf("Densité de liaison : %.2f\n\n", densite);

    // 4. Distribution des degrés
    int degres[nombre_noeuds];
    for (int i = 0; i < nombre_noeuds; i++) {
        degres[i] = 0;
    }

    for (int i = 0; i < nombre_arcs; i++) {
        degres[arcs[i].source]++;
        degres[arcs[i].destination]++;
    }

    printf("Distribution des degrés :\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("Sommet %d : Degré %d\n", i, degres[i]);
    }
}

// Fonction DFS pour la vérification de la connexité
void dfs(int sommet, int *visited, int adj[MAX_NODES][MAX_NODES], int nombre_noeuds) {
    visited[sommet] = 1;
    for (int i = 0; i < nombre_noeuds; i++) {
        if (adj[sommet][i] && !visited[i]) {
            dfs(i, visited, adj, nombre_noeuds);
        }
    }
}

// Fonction pour vérifier la connexité faible
int verifier_connexite_faible(Arcs *arcs, int nombre_arcs, int nombre_noeuds) {
    // Matrice d'adjacence non dirigée
    int adj[MAX_NODES][MAX_NODES] = {0};

    // Remplissage de la matrice d'adjacence
    for (int i = 0; i < nombre_arcs; i++) {
        adj[arcs[i].source][arcs[i].destination] = 1;
        adj[arcs[i].destination][arcs[i].source] = 1; // Ignorer la direction
    }

    // Tableau pour marquer les sommets visités
    int visited[MAX_NODES] = {0};

    // Lancer DFS depuis le sommet 0
    dfs(0, visited, adj, nombre_noeuds);

    // Vérifier si tous les sommets ont été visités
    for (int i = 0; i < nombre_noeuds; i++) {
        if (!visited[i]) {
            return 0; // Le graphe n'est pas connexe
        }
    }
    return 1; // Le graphe est connexe
}

// Charger le réseau et initialiser noeud_count et arc_count
void charger_et_afficher_reseau(const char *nom_fichier, Noeud *noeuds, Arcs *arcs, int *noeud_count, int *arc_count) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strncmp(ligne, "# Nœuds", 7) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (sscanf(ligne, "%d %s %s %lf %lf %lf", &noeuds[*noeud_count].id, noeuds[*noeud_count].nom, noeuds[*noeud_count].type, &noeuds[*noeud_count].r, &noeuds[*noeud_count].K, &noeuds[*noeud_count].N) == 6) {
                    (*noeud_count)++;
                }
            }
        }
        if (strncmp(ligne, "# Arcs", 6) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (sscanf(ligne, "%d %d %d %f", &arcs[*arc_count].id, &arcs[*arc_count].source, &arcs[*arc_count].destination, &arcs[*arc_count].poids) == 4) {
                    (*arc_count)++;
                }
            }
        }
    }

    fclose(fichier);

    // Affichage des informations
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < *noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }

    printf("\n=== Affichage des arcs ===\n");
    for (int i = 0; i < *arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    printf("\n=== Affichage des successeurs et prédécesseurs ===");
    afficher_adjacence(noeuds, arcs, *noeud_count, *arc_count);
}
void afficher_niveaux_trophiques(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int espece_id) {
    int niveaux[MAX_NODES];
    for (int i = 0; i < nombre_noeuds; i++) {
        niveaux[i] = -1; // Initialisation
    }
    niveaux[0] = 0; // Producteur primaire au niveau 0

    for (int i = 0; i < nombre_arcs; i++) {
        int source = arcs[i].source;
        int destination = arcs[i].destination;
        if (niveaux[source] != -1 && niveaux[destination] < niveaux[source] + 1) {
            niveaux[destination] = niveaux[source] + 1;
        }
    }

    if (niveaux[espece_id] != -1) {
        printf("\nNiveau trophique de %s : %d\n", noeuds[espece_id].nom, niveaux[espece_id]);
    } else {
        printf("\nNiveau trophique non calculable pour %s.\n", noeuds[espece_id].nom);
    }
}

void afficher_graphe_ascendants_recursif(Noeud *noeuds, Arcs *arcs, int nombre_arcs, int espece_id, int niveau) {
    int found = 0;
    for (int i = 0; i < nombre_arcs; i++) {
        if (arcs[i].destination == espece_id) {
            found = 1;
            for (int j = 0; j < niveau; j++) {
                printf("  "); // Indentation pour montrer le niveau
            }
            printf("%s <- %s\n", noeuds[espece_id].nom, noeuds[arcs[i].source].nom);
            afficher_graphe_ascendants_recursif(noeuds, arcs, nombre_arcs, arcs[i].source, niveau + 1);
        }
    }

    if (!found && niveau == 0) {
        printf("Aucune chaîne alimentaire trouvée pour %s.\n", noeuds[espece_id].nom);
    }
}

void afficher_graphe_ascendants(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int espece_id) {
    printf("\nChaînes alimentaires complètes dont dépend %s :\n", noeuds[espece_id].nom);
    afficher_graphe_ascendants_recursif(noeuds, arcs, nombre_arcs, espece_id, 0);
}

// Fonction pour calculer les degrés (entrée et sortie)
void calculer_degres(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    int in_degree[MAX_NODES] = {0};
    int out_degree[MAX_NODES] = {0};

    for (int i = 0; i < nombre_arcs; i++) {
        out_degree[arcs[i].source]++;
        in_degree[arcs[i].destination]++;
    }

    printf("\n=== Degrés des espèces ===\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("%s: in-degree = %d, out-degree = %d\n", noeuds[i].nom, in_degree[i], out_degree[i]);
    }
}

// Fonction pour calculer la centralité d'intermédiarité
void calculer_centralite_intermediarite(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    float centralite[MAX_NODES] = {0};
    int adj[MAX_NODES][MAX_NODES] = {0};

    // Construire la matrice d'adjacence
    for (int i = 0; i < nombre_arcs; i++) {
        adj[arcs[i].source][arcs[i].destination] = 1;
    }

    // Calcul de la centralité d'intermédiarité (simplifiée)
    for (int k = 0; k < nombre_noeuds; k++) {
        for (int i = 0; i < nombre_noeuds; i++) {
            for (int j = 0; j < nombre_noeuds; j++) {
                if (i != j && i != k && j != k && adj[i][k] && adj[k][j]) {
                    centralite[k]++;
                }
            }
        }
    }

    printf("\n=== Centralité d'intermédiarité ===\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("%s: centralité = %.2f\n", noeuds[i].nom, centralite[i]);
    }
}

// Fonction pour simuler la disparition d'une espèce
void simuler_disparition(Noeud *noeuds, Arcs *arcs, int *nombre_arcs, int espece_id) {
    printf("\n=== Simulation de la disparition de %s ===\n", noeuds[espece_id].nom);

    // Afficher les arcs liés à l'espèce disparue
    printf("\nArcs liés à %s avant la disparition :\n", noeuds[espece_id].nom);
    for (int i = 0; i < *nombre_arcs; i++) {
        if (arcs[i].source == espece_id || arcs[i].destination == espece_id) {
            afficher_arc(&arcs[i]);
        }
    }

    // Suppression des arcs liés à l'espèce disparue
    int nouveaux_arcs[MAX_ARCS];
    int compteur = 0;
    for (int i = 0; i < *nombre_arcs; i++) {
        if (arcs[i].source != espece_id && arcs[i].destination != espece_id) {
            arcs[compteur++] = arcs[i]; // Garder l'arc
        }
    }
    *nombre_arcs = compteur; // Mettre à jour le nombre d'arcs après suppression

    // Afficher les arcs restants après suppression
    printf("\nArcs après la disparition de %s :\n", noeuds[espece_id].nom);
    for (int i = 0; i < *nombre_arcs; i++) {
        afficher_arc(&arcs[i]);
    }
}

// Function to get the terminal width on Windows
int get_terminal_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    return columns;
}

// Function to get the terminal height on Windows
int get_terminal_height() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return rows;
}

// Function to center and print text
void print_centered(const char *text) {
    int terminal_width = get_terminal_width();
    int text_length = strlen(text);
    int padding = (terminal_width - text_length) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

// Function to center and print text exactly in the middle of the console
void text_centered(const char *text) {
    int terminal_width = get_terminal_width();
    int terminal_height = get_terminal_height();
    int text_length = strlen(text);
    int padding_left = (terminal_width - text_length) / 2;
    int padding_top = terminal_height / 2;

    // Move the cursor to the middle of the console
    COORD coord;
    coord.X = padding_left;
    coord.Y = padding_top;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // Print the centered text
    printf("%s\n", text);
}

// Function to center the cursor on the screen
void center_cursor() {
    int terminal_width = get_terminal_width();
    int terminal_height = get_terminal_height();
    COORD coord;
    coord.X = terminal_width / 2 - 7;
    coord.Y = (terminal_height / 2) + 5; // Move down by 5 lines
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void afficher_loading(const char *message) {
    system("cls"); // Clear the screen
    text_centered(message);
    COORD coord;
    coord.X = get_terminal_width() / 2 + strlen(message) / 2;
    coord.Y = get_terminal_height() / 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    for (int i = 0; i < 3; i++) {
        Sleep(500); // Wait for 1 second
        printf(".");
        fflush(stdout); // Ensure the dot is printed immediately
    }
    Sleep(1000); // Wait for 1 second before proceeding
    system("cls"); // Clear the screen
}

// Fonction pour mettre à jour la population basée sur la croissance logistique
void mettre_a_jour_population(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int iteration) {
    for (int i = 0; i < nombre_noeuds; i++) {
        // Initialisation correcte des valeurs de biomasse et flux énergétique
        if (noeuds[i].biomasse < 0) {
            noeuds[i].biomasse = 0;
        }
        if (noeuds[i].flux_energie < 0 || noeuds[i].flux_energie > 1000.0) {
            noeuds[i].flux_energie = 0; // Set to 0 if the value is unrealistic
        }

        // Calcul de la croissance logistique avec variation saisonnière et aléatoire
        double seasonal_factor = 1.0 - 0.1 * sin(iteration * 0.1); // Variation saisonnière (réduction)
        double random_factor = 1.0 - ((rand() % 21) / 100.0); // Variation aléatoire entre 0% et -20%
        double croissance = noeuds[i].r * noeuds[i].N * (1 - noeuds[i].N / noeuds[i].K) * seasonal_factor * random_factor;

        // Prendre en compte les interactions des arcs : prédation et ressources
        for (int j = 0; j < nombre_arcs; j++) {
            if (arcs[j].source == noeuds[i].id) {
                int dest = arcs[j].destination;
                double poids = arcs[j].poids;
                croissance -= poids * noeuds[dest].N * 0.1; // Impact de la prédation
            }
        }

        // Facteur humain impactant
        if (strcmp(noeuds[i].type, "Habitat") == 0) {
            croissance *= 0.5; // Réduire la croissance de 50% pour les habitats affectés par l'activité humaine
        }

        // Facteur de mortalité basé sur la densité de population
        double mortality_factor = 1.0 + 0.05 * (noeuds[i].N / noeuds[i].K); // Augmente avec la densité de population
        croissance /= mortality_factor; // Appliquer le facteur de mortalité

        // Mise à jour de la population après les interactions
        noeuds[i].N += croissance;

        // Limite la population à la capacité de portage
        if (noeuds[i].N > noeuds[i].K) {
            noeuds[i].N = noeuds[i].K;
        } else if (noeuds[i].N < 0) {
            noeuds[i].N = 0;
        }

        // Homogénéisation des biomasses et flux énergétiques
        noeuds[i].biomasse = noeuds[i].N * 0.1; // Exemple : 0.1 kg par unité de population
        noeuds[i].flux_energie = noeuds[i].N * 0.05; // Exemple : 0.05 W par unité de population

        // Correction des valeurs de flux énergétique pour éviter des valeurs irréalistes
        if (noeuds[i].flux_energie > 1000.0) {
            noeuds[i].flux_energie = 1000.0;
        }
    }
}

int animation_en_cours = 0; // 0: pause, 1: en cours
void lancer_animation() {
    animation_en_cours = 1;
}
void pauser_animation() {
    animation_en_cours = 0;
}

void modifier_valeurs(Noeud *noeuds, int nombre_noeuds) {
    int id;
    double nouvelle_valeur;
    printf("Entrez l'ID du sommet à modifier : ");
    scanf("%d", &id);
    if (id < 0 || id >= nombre_noeuds) {
        printf("ID invalide.\n");
        return;
    }
    printf("Entrez la nouvelle valeur de la population : ");
    scanf("%lf", &nouvelle_valeur);
    noeuds[id].N = nouvelle_valeur;
    printf("Population du sommet %d mise à jour à %.2f\n", id, noeuds[id].N);
}

void neutraliser_sommet(Noeud *noeuds, int nombre_noeuds) {
    int id;
    printf("Entrez l'ID du sommet à neutraliser : ");
    scanf("%d", &id);
    if (id < 0 || id >= nombre_noeuds) {
        printf("ID invalide.\n");
        return;
    }
    noeuds[id].N = 0;
    printf("Sommet %d neutralisé.\n", id);
}

// Fonction pour simuler l'évolution de la population au fil du temps
void simuler_evolution_population(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    int t = 0;
    while (1) {
        mettre_a_jour_population(noeuds, arcs, nombre_noeuds, nombre_arcs, t);

        // Affichage de l'état du réseau après chaque itération
        printf("Iteration %d:\n", t + 1);
        for (int i = 0; i < nombre_noeuds; i++) {
            printf("Sommet %d: %s - Population: %.2f\n", noeuds[i].id, noeuds[i].nom, noeuds[i].N);
        }
        printf("\n");

        printf("Animation en pause. Appuyez sur 'c' pour continuer, 'm' pour modifier les valeurs, 'n' pour neutraliser un sommet, 'q' pour quitter.\n");
        char choix = _getch();
        if (choix == 'c') {
            t++; // Avancer l'itération
        } else if (choix == 'm') {
            modifier_valeurs(noeuds, nombre_noeuds);
        } else if (choix == 'n') {
            neutraliser_sommet(noeuds, nombre_noeuds);
        } else if (choix == 'q') {
            break;
        }
    }
}
// Fonction pour afficher le réseau, les nœuds et les arcs
void afficher_reseau(Noeud *noeuds, Arcs *arcs, int noeud_count, int arc_count) {
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }

    printf("\n=== Affichage des arcs ===\n");
    for (int i = 0; i < arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    printf("\n=== Affichage des successeurs et prédécesseurs ===");
    afficher_adjacence(noeuds, arcs, noeud_count, arc_count);
}

// Fonction pour charger le réseau depuis un fichier
void charger_reseau(const char *nom_fichier, Noeud *noeuds, Arcs *arcs, int *noeud_count, int *arc_count) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }

    char ligne[256];
    *noeud_count = 0;
    *arc_count = 0;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strncmp(ligne, "# Nœuds", 7) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (*noeud_count >= MAX_NODES) {
                    fclose(fichier);
                    return;
                }
                if (sscanf(ligne, "%d %s %s %lf %lf %lf", &noeuds[*noeud_count].id, noeuds[*noeud_count].nom, noeuds[*noeud_count].type, &noeuds[*noeud_count].r, &noeuds[*noeud_count].K, &noeuds[*noeud_count].N) == 6) {
                    (*noeud_count)++;
                }
            }
        }
        if (strncmp(ligne, "# Arcs", 6) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (*arc_count >= MAX_ARCS) {
                    fclose(fichier);
                    return;
                }
                if (sscanf(ligne, "%d %d %d %f", &arcs[*arc_count].id, &arcs[*arc_count].source, &arcs[*arc_count].destination, &arcs[*arc_count].poids) == 4) {
                    (*arc_count)++;
                }
            }
        }
    }

    fclose(fichier);
}

void runge_kutta_2(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, double dt) {
    for (int i = 0; i < nombre_noeuds; i++) {
        double k1 = noeuds[i].r * noeuds[i].N * (1 - noeuds[i].N / noeuds[i].K);
        double k2 = noeuds[i].r * (noeuds[i].N + dt * k1 / 2) * (1 - (noeuds[i].N + dt * k1 / 2) / noeuds[i].K);
        noeuds[i].N += dt * k2;
        if (noeuds[i].N > noeuds[i].K) {
            noeuds[i].N = noeuds[i].K;
        } else if (noeuds[i].N < 0) {
            noeuds[i].N = 0;
        }
    }
}

void appliquer_facteurs_anthropiques(Noeud *noeuds, int nombre_noeuds) {
    for (int i = 0; i < nombre_noeuds; i++) {
        if (strcmp(noeuds[i].type, "Habitat") == 0) {
            noeuds[i].K *= 0.5;  // Exemple : réduire la capacité de portage de 50%
        }
    }
}

void plot_population(Noeud *noeuds, int nombre_noeuds, int iteration) {
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    fprintf(gnuplot, "set terminal png\n");
    fprintf(gnuplot, "set output 'population_%d.png'\n", iteration);
    fprintf(gnuplot, "plot '-' using 1:2 with lines title 'Population'\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        fprintf(gnuplot, "%d %.2f\n", i, noeuds[i].N);
    }
    fprintf(gnuplot, "e\n");
    pclose(gnuplot);
}

void afficher_rapport(Noeud *noeuds, Arcs *arcs, int noeud_count, int arc_count) {
    system("cls");
    printf("\n=== Rapport de Synthèse ===\n");

    // Affichage des sommets et arcs
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }
    for (int i = 0; i < arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    // Affichage des successeurs et prédécesseurs
    printf("\n=== Affichage des successeurs et prédécesseurs ===\n");
    for (int i = 0; i < noeud_count; i++) {
        printf("Sommet %d: %s\nSuccesseurs:", noeuds[i].id, noeuds[i].nom);
        for (int j = 0; j < arc_count; j++) {
            if (arcs[j].source == noeuds[i].id) {
                printf(" %d", arcs[j].destination);
            }
        }
        printf("\nPrédécesseurs:");
        for (int j = 0; j < arc_count; j++) {
            if (arcs[j].destination == noeuds[i].id) {
                printf(" %d", arcs[j].source);
            }
        }
        printf("\n");
    }

    // Espèces avec plusieurs prédateurs
    afficher_especes_avec_multiple_predateurs(arcs, arc_count, noeuds, noeud_count);

    // Espèces avec plusieurs sources d'alimentation
    afficher_especes_avec_multiple_sources(arcs, arc_count, noeuds, noeud_count);

    // Espèces les plus connectées
    afficher_especes_plus_connectees(arcs, arc_count, noeuds, noeud_count);

    // Espèces avec des prédateurs et des proies
    afficher_especes_avec_predateurs_et_proies(arcs, arc_count, noeuds, noeud_count);

    // Noeuds isolés
    afficher_noeuds_isoles(arcs, arc_count, noeuds, noeud_count);

    // Complexité du réseau
    afficher_complexite(noeuds, arcs, noeud_count, arc_count);

    // Vérification de la connexité
    int connexe = verifier_connexite_faible(arcs, arc_count, noeud_count);
    printf("\nLe graphe est %sconnexe.\n", connexe ? "" : "non ");

    // Résultats de la simulation
    printf("\n=== Résultats de la Simulation ===\n");
    for (int i = 0; i < noeud_count; i++) {
        printf("Sommet %d: %s (%s) - Population: %.2f\n", noeuds[i].id, noeuds[i].nom, noeuds[i].type, noeuds[i].N);
    }
}

int main() {
    Noeud noeuds[MAX_NODES];
    Arcs arcs[MAX_ARCS];
    int noeud_count = 0, arc_count = 0;  // Initialisation de noeud_count et arc_count

    char choix[256];
    char espece_nom[100];  // Déclare une seule fois l'input pour l'espèce
    int espece_id = -1;     // Déclare une seule fois l'ID de l'espèce
    char nomfichier[256];   // Déclare la variable nomfichier

    while (1) {
        system("cls"); // Clear the screen
        printf("\t\t\t\t\t _______              _     _\n");
        printf("\t\t\t\t\t|__   __|            | |   (_)\n");
        printf("\t\t\t\t\t   | |_ __ ___  _ __ | |__  _  ___\n");
        printf("\t\t\t\t\t   | | '__/ _ \\| '_ \\| '_ \\| |/ __|\n");
        printf("\t\t\t\t\t   | | | | (_) | |_) | | | | | (__\n");
        printf("\t\t\t\t\t _ |_|_|  \\___/| .__/|_| |_|_|\\___|_\n");
        printf("\t\t\t\t\t| \\ | |    | | | |                | |\n");
        printf("\t\t\t\t\t|  \\| | ___| |_|_|     _____  _ __| | _____\n");
        printf("\t\t\t\t\t| . ` |/ _ \\ __\\ \\ /\\ / / _ \\| '__| |/ / __|\n");
        printf("\t\t\t\t\t| |\\  |  __/ |_ \\ V  V / (_) | |  |   <\\__ \\\n");
        printf("\t\t\t\t\t|_| \\_|\\___|\\__| \\_/\\_/ \\___/|_|  |_|\\_\\___/\n\n");

        print_centered("Choisir un réseau trophique:\n\n");
        print_centered("1. Foret temperee");
        print_centered("2. Ocean Arctique");
        print_centered("3. Savane Africaine");
        print_centered("4. Quitter");
        center_cursor();
        fgets(choix, sizeof(choix), stdin);
        choix[strcspn(choix, "\n")] = '\0'; // Retirer le caractère de nouvelle ligne

        if (strcmp(choix, "Foret temperee") == 0) {
            afficher_loading("Loading");
            strcpy(nomfichier, "graphe1.txt");
        } else if (strcmp(choix, "Ocean Arctique") == 0) {
            afficher_loading("Loading");
            strcpy(nomfichier, "graphe2.txt");
        } else if (strcmp(choix, "Savane Africaine") == 0) {
            afficher_loading("Loading");
            strcpy(nomfichier, "graphe3.txt");
        } else if (strcmp(choix, "Quitter") == 0) {
            break;
        } else {
            afficher_loading("Erreur");
            continue;
        }

        charger_et_afficher_reseau(nomfichier, noeuds, arcs, &noeud_count, &arc_count);

        // Demander à l'utilisateur de sélectionner une espèce par son nom
        printf("\nEntrez le nom de l'espèce pour afficher les chaînes alimentaires dont elle dépend : ");
        scanf("%s", espece_nom);

        // Trouver l'ID correspondant au nom entré
        espece_id = -1;
        for (int i = 0; i < noeud_count; i++) {
            if (strcmp(noeuds[i].nom, espece_nom) == 0) {
                espece_id = noeuds[i].id;
                break;
            }
        }

        if (espece_id != -1) {
            afficher_graphe_ascendants(noeuds, arcs, noeud_count, arc_count, espece_id);
            afficher_niveaux_trophiques(noeuds, arcs, noeud_count, arc_count, espece_id);
        } else {
            printf("Espèce \"%s\" introuvable dans le réseau.\n", espece_nom);
        }

        // Demander à l'utilisateur de simuler la disparition d'une espèce
        printf("\nEntrez le nom de l'espèce à simuler pour disparition : ");
        scanf("%s", espece_nom);

        // Trouver l'ID correspondant à l'espèce
        espece_id = -1;
        for (int i = 0; i < noeud_count; i++) {
            if (strcmp(noeuds[i].nom, espece_nom) == 0) {
                espece_id = i;
                break;
            }
        }

        if (espece_id != -1) {
            simuler_disparition(noeuds, arcs, &arc_count, espece_id);
            calculer_degres(noeuds, arcs, noeud_count, arc_count); // Recalcul des degrés
            calculer_centralite_intermediarite(noeuds, arcs, noeud_count, arc_count); // Recalcul de la centralité
        } else {
            printf("Espèce \"%s\" introuvable.\n", espece_nom);
        }
        charger_reseau(nomfichier, noeuds, arcs, &noeud_count, &arc_count);
        simuler_evolution_population(noeuds, arcs, noeud_count, arc_count);
        system("cls");
        afficher_rapport(noeuds, arcs, noeud_count, arc_count);

        while (_getch() != 27); // 27 is the ASCII code for Esc
    }

    return 0;
}
  // 3. Densité de liaison
    float densite = (float)nombre_arcs / (nombre_noeuds * (nombre_noeuds - 1));
    printf("Densité de liaison : %.2f\n\n", densite);

    // 4. Distribution des degrés
    int degres[nombre_noeuds];
    for (int i = 0; i < nombre_noeuds; i++) {
        degres[i] = 0;
    }

    for (int i = 0; i < nombre_arcs; i++) {
        degres[arcs[i].source]++;
        degres[arcs[i].destination]++;
    }

    printf("Distribution des degrés :\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("Sommet %d : Degré %d\n", i, degres[i]);
    }
}

// Fonction DFS pour la vérification de la connexité
void dfs(int sommet, int *visited, int adj[MAX_NODES][MAX_NODES], int nombre_noeuds) {
    visited[sommet] = 1;
    for (int i = 0; i < nombre_noeuds; i++) {
        if (adj[sommet][i] && !visited[i]) {
            dfs(i, visited, adj, nombre_noeuds);
        }
    }
}

// Fonction pour vérifier la connexité faible
int verifier_connexite_faible(Arcs *arcs, int nombre_arcs, int nombre_noeuds) {
    // Matrice d'adjacence non dirigée
    int adj[MAX_NODES][MAX_NODES] = {0};

    // Remplissage de la matrice d'adjacence
    for (int i = 0; i < nombre_arcs; i++) {
        adj[arcs[i].source][arcs[i].destination] = 1;
        adj[arcs[i].destination][arcs[i].source] = 1; // Ignorer la direction
    }

    // Tableau pour marquer les sommets visités
    int visited[MAX_NODES] = {0};

    // Lancer DFS depuis le sommet 0
    dfs(0, visited, adj, nombre_noeuds);

    // Vérifier si tous les sommets ont été visités
    for (int i = 0; i < nombre_noeuds; i++) {
        if (!visited[i]) {
            return 0; // Le graphe n'est pas connexe
        }
    }
    return 1; // Le graphe est connexe
}

// Charger le réseau et initialiser noeud_count et arc_count
void charger_et_afficher_reseau(const char *nom_fichier, Noeud *noeuds, Arcs *arcs, int *noeud_count, int *arc_count) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strncmp(ligne, "# Nœuds", 7) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (sscanf(ligne, "%d %s %s %lf %lf %lf", &noeuds[*noeud_count].id, noeuds[*noeud_count].nom, noeuds[*noeud_count].type, &noeuds[*noeud_count].r, &noeuds[*noeud_count].K, &noeuds[*noeud_count].N) == 6) {
                    (*noeud_count)++;
                }
            }
        }
        if (strncmp(ligne, "# Arcs", 6) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (sscanf(ligne, "%d %d %d %f", &arcs[*arc_count].id, &arcs[*arc_count].source, &arcs[*arc_count].destination, &arcs[*arc_count].poids) == 4) {
                    (*arc_count)++;
                }
            }
        }
    }

    fclose(fichier);

    // Affichage des informations
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < *noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }

    printf("\n=== Affichage des arcs ===\n");
    for (int i = 0; i < *arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    printf("\n=== Affichage des successeurs et prédécesseurs ===");
    afficher_adjacence(noeuds, arcs, *noeud_count, *arc_count);
}
void afficher_niveaux_trophiques(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int espece_id) {
    int niveaux[MAX_NODES];
    for (int i = 0; i < nombre_noeuds; i++) {
        niveaux[i] = -1; // Initialisation
    }
    niveaux[0] = 0; // Producteur primaire au niveau 0

    for (int i = 0; i < nombre_arcs; i++) {
        int source = arcs[i].source;
        int destination = arcs[i].destination;
        if (niveaux[source] != -1 && niveaux[destination] < niveaux[source] + 1) {
            niveaux[destination] = niveaux[source] + 1;
        }
    }

    if (niveaux[espece_id] != -1) {
        printf("\nNiveau trophique de %s : %d\n", noeuds[espece_id].nom, niveaux[espece_id]);
    } else {
        printf("\nNiveau trophique non calculable pour %s.\n", noeuds[espece_id].nom);
    }
}

void afficher_graphe_ascendants_recursif(Noeud *noeuds, Arcs *arcs, int nombre_arcs, int espece_id, int niveau) {
    int found = 0;
    for (int i = 0; i < nombre_arcs; i++) {
        if (arcs[i].destination == espece_id) {
            found = 1;
            for (int j = 0; j < niveau; j++) {
                printf("  "); // Indentation pour montrer le niveau
            }
            printf("%s <- %s\n", noeuds[espece_id].nom, noeuds[arcs[i].source].nom);
            afficher_graphe_ascendants_recursif(noeuds, arcs, nombre_arcs, arcs[i].source, niveau + 1);
        }
    }

    if (!found && niveau == 0) {
        printf("Aucune chaîne alimentaire trouvée pour %s.\n", noeuds[espece_id].nom);
    }
}

void afficher_graphe_ascendants(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int espece_id) {
    printf("\nChaînes alimentaires complètes dont dépend %s :\n", noeuds[espece_id].nom);
    afficher_graphe_ascendants_recursif(noeuds, arcs, nombre_arcs, espece_id, 0);
}

// Fonction pour calculer les degrés (entrée et sortie)
void calculer_degres(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    int in_degree[MAX_NODES] = {0};
    int out_degree[MAX_NODES] = {0};

    for (int i = 0; i < nombre_arcs; i++) {
        out_degree[arcs[i].source]++;
        in_degree[arcs[i].destination]++;
    }

    printf("\n=== Degrés des espèces ===\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("%s: in-degree = %d, out-degree = %d\n", noeuds[i].nom, in_degree[i], out_degree[i]);
    }
}

// Fonction pour calculer la centralité d'intermédiarité
void calculer_centralite_intermediarite(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    float centralite[MAX_NODES] = {0};
    int adj[MAX_NODES][MAX_NODES] = {0};

    // Construire la matrice d'adjacence
    for (int i = 0; i < nombre_arcs; i++) {
        adj[arcs[i].source][arcs[i].destination] = 1;
    }

    // Calcul de la centralité d'intermédiarité (simplifiée)
    for (int k = 0; k < nombre_noeuds; k++) {
        for (int i = 0; i < nombre_noeuds; i++) {
            for (int j = 0; j < nombre_noeuds; j++) {
                if (i != j && i != k && j != k && adj[i][k] && adj[k][j]) {
                    centralite[k]++;
                }
            }
        }
    }

    printf("\n=== Centralité d'intermédiarité ===\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("%s: centralité = %.2f\n", noeuds[i].nom, centralite[i]);
    }
}

// Fonction pour simuler la disparition d'une espèce
void simuler_disparition(Noeud *noeuds, Arcs *arcs, int *nombre_arcs, int espece_id) {
    printf("\n=== Simulation de la disparition de %s ===\n", noeuds[espece_id].nom);

    // Afficher les arcs liés à l'espèce disparue
    printf("\nArcs liés à %s avant la disparition :\n", noeuds[espece_id].nom);
    for (int i = 0; i < *nombre_arcs; i++) {
        if (arcs[i].source == espece_id || arcs[i].destination == espece_id) {
            afficher_arc(&arcs[i]);
        }
    }

    // Suppression des arcs liés à l'espèce disparue
    int nouveaux_arcs[MAX_ARCS];
    int compteur = 0;
    for (int i = 0; i < *nombre_arcs; i++) {
        if (arcs[i].source != espece_id && arcs[i].destination != espece_id) {
            arcs[compteur++] = arcs[i]; // Garder l'arc
        }
    }
    *nombre_arcs = compteur; // Mettre à jour le nombre d'arcs après suppression

    // Afficher les arcs restants après suppression
    printf("\nArcs après la disparition de %s :\n", noeuds[espece_id].nom);
    for (int i = 0; i < *nombre_arcs; i++) {
        afficher_arc(&arcs[i]);
    }
}

// Function to get the terminal width on Windows
int get_terminal_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    return columns;
}

// Function to get the terminal height on Windows
int get_terminal_height() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return rows;
}

// Function to center and print text
void print_centered(const char *text) {
    int terminal_width = get_terminal_width();
    int text_length = strlen(text);
    int padding = (terminal_width - text_length) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

// Function to center and print text exactly in the middle of the console
void text_centered(const char *text) {
    int terminal_width = get_terminal_width();
    int terminal_height = get_terminal_height();
    int text_length = strlen(text);
    int padding_left = (terminal_width - text_length) / 2;
    int padding_top = terminal_height / 2;

    // Move the cursor to the middle of the console
    COORD coord;
    coord.X = padding_left;
    coord.Y = padding_top;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // Print the centered text
    printf("%s\n", text);
}

// Function to center the cursor on the screen
void center_cursor() {
    int terminal_width = get_terminal_width();
    int terminal_height = get_terminal_height();
    COORD coord;
    coord.X = terminal_width / 2 - 7;
    coord.Y = (terminal_height / 2) + 5; // Move down by 5 lines
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void afficher_loading(const char *message) {
    system("cls"); // Clear the screen
    text_centered(message)
  // 3. Densité de liaison
    float densite = (float)nombre_arcs / (nombre_noeuds * (nombre_noeuds - 1));
    printf("Densité de liaison : %.2f\n\n", densite);

    // 4. Distribution des degrés
    int degres[nombre_noeuds];
    for (int i = 0; i < nombre_noeuds; i++) {
        degres[i] = 0;
    }

    for (int i = 0; i < nombre_arcs; i++) {
        degres[arcs[i].source]++;
        degres[arcs[i].destination]++;
    }

    printf("Distribution des degrés :\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("Sommet %d : Degré %d\n", i, degres[i]);
    }
}

// Fonction DFS pour la vérification de la connexité
void dfs(int sommet, int *visited, int adj[MAX_NODES][MAX_NODES], int nombre_noeuds) {
    visited[sommet] = 1;
    for (int i = 0; i < nombre_noeuds; i++) {
        if (adj[sommet][i] && !visited[i]) {
            dfs(i, visited, adj, nombre_noeuds);
        }
    }
}

// Fonction pour vérifier la connexité faible
int verifier_connexite_faible(Arcs *arcs, int nombre_arcs, int nombre_noeuds) {
    // Matrice d'adjacence non dirigée
    int adj[MAX_NODES][MAX_NODES] = {0};

    // Remplissage de la matrice d'adjacence
    for (int i = 0; i < nombre_arcs; i++) {
        adj[arcs[i].source][arcs[i].destination] = 1;
        adj[arcs[i].destination][arcs[i].source] = 1; // Ignorer la direction
    }

    // Tableau pour marquer les sommets visités
    int visited[MAX_NODES] = {0};

    // Lancer DFS depuis le sommet 0
    dfs(0, visited, adj, nombre_noeuds);

    // Vérifier si tous les sommets ont été visités
    for (int i = 0; i < nombre_noeuds; i++) {
        if (!visited[i]) {
            return 0; // Le graphe n'est pas connexe
        }
    }
    return 1; // Le graphe est connexe
}

// Charger le réseau et initialiser noeud_count et arc_count
void charger_et_afficher_reseau(const char *nom_fichier, Noeud *noeuds, Arcs *arcs, int *noeud_count, int *arc_count) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strncmp(ligne, "# Nœuds", 7) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (sscanf(ligne, "%d %s %s %lf %lf %lf", &noeuds[*noeud_count].id, noeuds[*noeud_count].nom, noeuds[*noeud_count].type, &noeuds[*noeud_count].r, &noeuds[*noeud_count].K, &noeuds[*noeud_count].N) == 6) {
                    (*noeud_count)++;
                }
            }
        }
        if (strncmp(ligne, "# Arcs", 6) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (sscanf(ligne, "%d %d %d %f", &arcs[*arc_count].id, &arcs[*arc_count].source, &arcs[*arc_count].destination, &arcs[*arc_count].poids) == 4) {
                    (*arc_count)++;
                }
            }
        }
    }

    fclose(fichier);

    // Affichage des informations
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < *noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }

    printf("\n=== Affichage des arcs ===\n");
    for (int i = 0; i < *arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    printf("\n=== Affichage des successeurs et prédécesseurs ===");
    afficher_adjacence(noeuds, arcs, *noeud_count, *arc_count);
}
void afficher_niveaux_trophiques(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int espece_id) {
    int niveaux[MAX_NODES];
    for (int i = 0; i < nombre_noeuds; i++) {
        niveaux[i] = -1; // Initialisation
    }
    niveaux[0] = 0; // Producteur primaire au niveau 0

    for (int i = 0; i < nombre_arcs; i++) {
        int source = arcs[i].source;
        int destination = arcs[i].destination;
        if (niveaux[source] != -1 && niveaux[destination] < niveaux[source] + 1) {
            niveaux[destination] = niveaux[source] + 1;
        }
    }

    if (niveaux[espece_id] != -1) {
        printf("\nNiveau trophique de %s : %d\n", noeuds[espece_id].nom, niveaux[espece_id]);
    } else {
        printf("\nNiveau trophique non calculable pour %s.\n", noeuds[espece_id].nom);
    }
}

void afficher_graphe_ascendants_recursif(Noeud *noeuds, Arcs *arcs, int nombre_arcs, int espece_id, int niveau) {
    int found = 0;
    for (int i = 0; i < nombre_arcs; i++) {
        if (arcs[i].destination == espece_id) {
            found = 1;
            for (int j = 0; j < niveau; j++) {
                printf("  "); // Indentation pour montrer le niveau
            }
            printf("%s <- %s\n", noeuds[espece_id].nom, noeuds[arcs[i].source].nom);
            afficher_graphe_ascendants_recursif(noeuds, arcs, nombre_arcs, arcs[i].source, niveau + 1);
        }
    }

    if (!found && niveau == 0) {
        printf("Aucune chaîne alimentaire trouvée pour %s.\n", noeuds[espece_id].nom);
    }
}

void afficher_graphe_ascendants(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int espece_id) {
    printf("\nChaînes alimentaires complètes dont dépend %s :\n", noeuds[espece_id].nom);
    afficher_graphe_ascendants_recursif(noeuds, arcs, nombre_arcs, espece_id, 0);
}

// Fonction pour calculer les degrés (entrée et sortie)
void calculer_degres(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    int in_degree[MAX_NODES] = {0};
    int out_degree[MAX_NODES] = {0};

    for (int i = 0; i < nombre_arcs; i++) {
        out_degree[arcs[i].source]++;
        in_degree[arcs[i].destination]++;
    }

    printf("\n=== Degrés des espèces ===\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("%s: in-degree = %d, out-degree = %d\n", noeuds[i].nom, in_degree[i], out_degree[i]);
    }
}

// Fonction pour calculer la centralité d'intermédiarité
void calculer_centralite_intermediarite(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    float centralite[MAX_NODES] = {0};
    int adj[MAX_NODES][MAX_NODES] = {0};

    // Construire la matrice d'adjacence
    for (int i = 0; i < nombre_arcs; i++) {
        adj[arcs[i].source][arcs[i].destination] = 1;
    }

    // Calcul de la centralité d'intermédiarité (simplifiée)
    for (int k = 0; k < nombre_noeuds; k++) {
        for (int i = 0; i < nombre_noeuds; i++) {
            for (int j = 0; j < nombre_noeuds; j++) {
                if (i != j && i != k && j != k && adj[i][k] && adj[k][j]) {
                    centralite[k]++;
                }
            }
        }
    }

    printf("\n=== Centralité d'intermédiarité ===\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        printf("%s: centralité = %.2f\n", noeuds[i].nom, centralite[i]);
    }
}

// Fonction pour simuler la disparition d'une espèce
void simuler_disparition(Noeud *noeuds, Arcs *arcs, int *nombre_arcs, int espece_id) {
    printf("\n=== Simulation de la disparition de %s ===\n", noeuds[espece_id].nom);

    // Afficher les arcs liés à l'espèce disparue
    printf("\nArcs liés à %s avant la disparition :\n", noeuds[espece_id].nom);
    for (int i = 0; i < *nombre_arcs; i++) {
        if (arcs[i].source == espece_id || arcs[i].destination == espece_id) {
            afficher_arc(&arcs[i]);
        }
    }

    // Suppression des arcs liés à l'espèce disparue
    int nouveaux_arcs[MAX_ARCS];
    int compteur = 0;
    for (int i = 0; i < *nombre_arcs; i++) {
        if (arcs[i].source != espece_id && arcs[i].destination != espece_id) {
            arcs[compteur++] = arcs[i]; // Garder l'arc
        }
    }
    *nombre_arcs = compteur; // Mettre à jour le nombre d'arcs après suppression

    // Afficher les arcs restants après suppression
    printf("\nArcs après la disparition de %s :\n", noeuds[espece_id].nom);
    for (int i = 0; i < *nombre_arcs; i++) {
        afficher_arc(&arcs[i]);
    }
}

// Function to get the terminal width on Windows
int get_terminal_width() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    return columns;
}

// Function to get the terminal height on Windows
int get_terminal_height() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    return rows;
}

// Function to center and print text
void print_centered(const char *text) {
    int terminal_width = get_terminal_width();
    int text_length = strlen(text);
    int padding = (terminal_width - text_length) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }
    printf("%s\n", text);
}

// Function to center and print text exactly in the middle of the console
void text_centered(const char *text) {
    int terminal_width = get_terminal_width();
    int terminal_height = get_terminal_height();
    int text_length = strlen(text);
    int padding_left = (terminal_width - text_length) / 2;
    int padding_top = terminal_height / 2;

    // Move the cursor to the middle of the console
    COORD coord;
    coord.X = padding_left;
    coord.Y = padding_top;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // Print the centered text
    printf("%s\n", text);
}

// Function to center the cursor on the screen
void center_cursor() {
    int terminal_width = get_terminal_width();
    int terminal_height = get_terminal_height();
    COORD coord;
    coord.X = terminal_width / 2 - 7;
    coord.Y = (terminal_height / 2) + 5; // Move down by 5 lines
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void afficher_loading(const char *message) {
    system("cls"); // Clear the screen
    text_centered(message);
    COORD coord;
    coord.X = get_terminal_width() / 2 + strlen(message) / 2;
    coord.Y = get_terminal_height() / 2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    for (int i = 0; i < 3; i++) {
        Sleep(500); // Wait for 1 second
        printf(".");
        fflush(stdout); // Ensure the dot is printed immediately
    }
    Sleep(1000); // Wait for 1 second before proceeding
    system("cls"); // Clear the screen
}

// Fonction pour mettre à jour la population basée sur la croissance logistique
void mettre_a_jour_population(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, int iteration) {
    for (int i = 0; i < nombre_noeuds; i++) {
        // Initialisation correcte des valeurs de biomasse et flux énergétique
        if (noeuds[i].biomasse < 0) {
            noeuds[i].biomasse = 0;
        }
        if (noeuds[i].flux_energie < 0 || noeuds[i].flux_energie > 1000.0) {
            noeuds[i].flux_energie = 0; // Set to 0 if the value is unrealistic
        }

        // Calcul de la croissance logistique avec variation saisonnière et aléatoire
        double seasonal_factor = 1.0 - 0.1 * sin(iteration * 0.1); // Variation saisonnière (réduction)
        double random_factor = 1.0 - ((rand() % 21) / 100.0); // Variation aléatoire entre 0% et -20%
        double croissance = noeuds[i].r * noeuds[i].N * (1 - noeuds[i].N / noeuds[i].K) * seasonal_factor * random_factor;

        // Prendre en compte les interactions des arcs : prédation et ressources
        for (int j = 0; j < nombre_arcs; j++) {
            if (arcs[j].source == noeuds[i].id) {
                int dest = arcs[j].destination;
                double poids = arcs[j].poids;
                croissance -= poids * noeuds[dest].N * 0.1; // Impact de la prédation
            }
        }

        // Facteur humain impactant
        if (strcmp(noeuds[i].type, "Habitat") == 0) {
            croissance *= 0.5; // Réduire la croissance de 50% pour les habitats affectés par l'activité humaine
        }

        // Facteur de mortalité basé sur la densité de population
        double mortality_factor = 1.0 + 0.05 * (noeuds[i].N / noeuds[i].K); // Augmente avec la densité de population
        croissance /= mortality_factor; // Appliquer le facteur de mortalité

        // Mise à jour de la population après les interactions
        noeuds[i].N += croissance;

        // Limite la population à la capacité de portage
        if (noeuds[i].N > noeuds[i].K) {
            noeuds[i].N = noeuds[i].K;
        } else if (noeuds[i].N < 0) {
            noeuds[i].N = 0;
        }

        // Homogénéisation des biomasses et flux énergétiques
        noeuds[i].biomasse = noeuds[i].N * 0.1; // Exemple : 0.1 kg par unité de population
        noeuds[i].flux_energie = noeuds[i].N * 0.05; // Exemple : 0.05 W par unité de population

        // Correction des valeurs de flux énergétique pour éviter des valeurs irréalistes
        if (noeuds[i].flux_energie > 1000.0) {
            noeuds[i].flux_energie = 1000.0;
        }
    }
}

int animation_en_cours = 0; // 0: pause, 1: en cours
void lancer_animation() {
    animation_en_cours = 1;
}
void pauser_animation() {
    animation_en_cours = 0;
}

void modifier_valeurs(Noeud *noeuds, int nombre_noeuds) {
    int id;
    double nouvelle_valeur;
    printf("Entrez l'ID du sommet à modifier : ");
    scanf("%d", &id);
    if (id < 0 || id >= nombre_noeuds) {
        printf("ID invalide.\n");
        return;
    }
    printf("Entrez la nouvelle valeur de la population : ");
    scanf("%lf", &nouvelle_valeur);
    noeuds[id].N = nouvelle_valeur;
    printf("Population du sommet %d mise à jour à %.2f\n", id, noeuds[id].N);
}

void neutraliser_sommet(Noeud *noeuds, int nombre_noeuds) {
    int id;
    printf("Entrez l'ID du sommet à neutraliser : ");
    scanf("%d", &id);
    if (id < 0 || id >= nombre_noeuds) {
        printf("ID invalide.\n");
        return;
    }
    noeuds[id].N = 0;
    printf("Sommet %d neutralisé.\n", id);
}

// Fonction pour simuler l'évolution de la population au fil du temps
void simuler_evolution_population(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs) {
    int t = 0;
    while (1) {
        mettre_a_jour_population(noeuds, arcs, nombre_noeuds, nombre_arcs, t);

        // Affichage de l'état du réseau après chaque itération
        printf("Iteration %d:\n", t + 1);
        for (int i = 0; i < nombre_noeuds; i++) {
            printf("Sommet %d: %s - Population: %.2f\n", noeuds[i].id, noeuds[i].nom, noeuds[i].N);
        }
        printf("\n");

        printf("Animation en pause. Appuyez sur 'c' pour continuer, 'm' pour modifier les valeurs, 'n' pour neutraliser un sommet, 'q' pour quitter.\n");
        char choix = _getch();
        if (choix == 'c') {
            t++; // Avancer l'itération
        } else if (choix == 'm') {
            modifier_valeurs(noeuds, nombre_noeuds);
        } else if (choix == 'n') {
            neutraliser_sommet(noeuds, nombre_noeuds);
        } else if (choix == 'q') {
            break;
        }
    }
}
// Fonction pour afficher le réseau, les nœuds et les arcs
void afficher_reseau(Noeud *noeuds, Arcs *arcs, int noeud_count, int arc_count) {
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }

    printf("\n=== Affichage des arcs ===\n");
    for (int i = 0; i < arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    printf("\n=== Affichage des successeurs et prédécesseurs ===");
    afficher_adjacence(noeuds, arcs, noeud_count, arc_count);
}

// Fonction pour charger le réseau depuis un fichier
void charger_reseau(const char *nom_fichier, Noeud *noeuds, Arcs *arcs, int *noeud_count, int *arc_count) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (!fichier) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }

    char ligne[256];
    *noeud_count = 0;
    *arc_count = 0;

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (strncmp(ligne, "# Nœuds", 7) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (*noeud_count >= MAX_NODES) {
                    fclose(fichier);
                    return;
                }
                if (sscanf(ligne, "%d %s %s %lf %lf %lf", &noeuds[*noeud_count].id, noeuds[*noeud_count].nom, noeuds[*noeud_count].type, &noeuds[*noeud_count].r, &noeuds[*noeud_count].K, &noeuds[*noeud_count].N) == 6) {
                    (*noeud_count)++;
                }
            }
        }
        if (strncmp(ligne, "# Arcs", 6) == 0) {
            while (fgets(ligne, sizeof(ligne), fichier) && ligne[0] != '#') {
                if (*arc_count >= MAX_ARCS) {
                    fclose(fichier);
                    return;
                }
                if (sscanf(ligne, "%d %d %d %f", &arcs[*arc_count].id, &arcs[*arc_count].source, &arcs[*arc_count].destination, &arcs[*arc_count].poids) == 4) {
                    (*arc_count)++;
                }
            }
        }
    }

    fclose(fichier);
}

void runge_kutta_2(Noeud *noeuds, Arcs *arcs, int nombre_noeuds, int nombre_arcs, double dt) {
    for (int i = 0; i < nombre_noeuds; i++) {
        double k1 = noeuds[i].r * noeuds[i].N * (1 - noeuds[i].N / noeuds[i].K);
        double k2 = noeuds[i].r * (noeuds[i].N + dt * k1 / 2) * (1 - (noeuds[i].N + dt * k1 / 2) / noeuds[i].K);
        noeuds[i].N += dt * k2;
        if (noeuds[i].N > noeuds[i].K) {
            noeuds[i].N = noeuds[i].K;
        } else if (noeuds[i].N < 0) {
            noeuds[i].N = 0;
        }
    }
}

void appliquer_facteurs_anthropiques(Noeud *noeuds, int nombre_noeuds) {
    for (int i = 0; i < nombre_noeuds; i++) {
        if (strcmp(noeuds[i].type, "Habitat") == 0) {
            noeuds[i].K *= 0.5;  // Exemple : réduire la capacité de portage de 50%
        }
    }
}

void plot_population(Noeud *noeuds, int nombre_noeuds, int iteration) {
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    fprintf(gnuplot, "set terminal png\n");
    fprintf(gnuplot, "set output 'population_%d.png'\n", iteration);
    fprintf(gnuplot, "plot '-' using 1:2 with lines title 'Population'\n");
    for (int i = 0; i < nombre_noeuds; i++) {
        fprintf(gnuplot, "%d %.2f\n", i, noeuds[i].N);
    }
    fprintf(gnuplot, "e\n");
    pclose(gnuplot);
}

void afficher_rapport(Noeud *noeuds, Arcs *arcs, int noeud_count, int arc_count) {
    system("cls");
    printf("\n=== Rapport de Synthèse ===\n");

    // Affichage des sommets et arcs
    printf("\n=== Affichage des sommets et arcs ===\n");
    for (int i = 0; i < noeud_count; i++) {
        afficher_sommet(&noeuds[i]);
    }
    for (int i = 0; i < arc_count; i++) {
        afficher_arc(&arcs[i]);
    }

    // Affichage des successeurs et prédécesseurs
    printf("\n=== Affichage des successeurs et prédécesseurs ===\n");
    for (int i = 0; i < noeud_count; i++) {
        printf("Sommet %d: %s\nSuccesseurs:", noeuds[i].id, noeuds[i].nom);
        for (int j = 0; j < arc_count; j++) {
            if (arcs[j].source == noeuds[i].id) {
                printf(" %d", arcs[j].destination);
            }
        }
        printf("\nPrédécesseurs:");
        for (int j = 0; j < arc_count; j++) {
            if (arcs[j].destination == noeuds[i].id) {
                printf(" %d", arcs[j].source);
            }
        }
        printf("\n");
    }

    // Espèces avec plusieurs prédateurs
    afficher_especes_avec_multiple_predateurs(arcs, arc_count, noeuds, noeud_count);

    // Espèces avec plusieurs sources d'alimentation
    afficher_especes_avec_multiple_sources(arcs, arc_count, noeuds, noeud_count);

    // Espèces les plus connectées
    afficher_especes_plus_connectees(arcs, arc_count, noeuds, noeud_count);

    // Espèces avec des prédateurs et des proies
    afficher_especes_avec_predateurs_et_proies(arcs, arc_count, noeuds, noeud_count);

    // Noeuds isolés
    afficher_noeuds_isoles(arcs, arc_count, noeuds, noeud_count);

    // Complexité du réseau
    afficher_complexite(noeuds, arcs, noeud_count, arc_count);

    // Vérification de la connexité
    int connexe = verifier_connexite_faible(arcs, arc_count, noeud_count);
    printf("\nLe graphe est %sconnexe.\n", connexe ? "" : "non ");

    // Résultats de la simulation
    printf("\n=== Résultats de la Simulation ===\n");
    for (int i = 0; i < noeud_count; i++) {
        printf("Sommet %d: %s (%s) - Population: %.2f\n", noeuds[i].id, noeuds[i].nom, noeuds[i].type, noeuds[i].N);
    }
}

int main() {
    Noeud noeuds[MAX_NODES];
    Arcs arcs[MAX_ARCS];
    int noeud_count = 0, arc_count = 0;  // Initialisation de noeud_count et arc_count

    char choix[256];
    char espece_nom[100];  // Déclare une seule fois l'input pour l'espèce
    int espece_id = -1;     // Déclare une seule fois l'ID de l'espèce
    char nomfichier[256];   // Déclare la variable nomfichier

    while (1) {
        system("cls"); // Clear the screen
        printf("\t\t\t\t\t _______              _     _\n");
        printf("\t\t\t\t\t|__   __|            | |   (_)\n");
        printf("\t\t\t\t\t   | |_ __ ___  _ __ | |__  _  ___\n");
        printf("\t\t\t\t\t   | | '__/ _ \\| '_ \\| '_ \\| |/ __|\n");
        printf("\t\t\t\t\t   | | | | (_) | |_) | | | | | (__\n");
        printf("\t\t\t\t\t _ |_|_|  \\___/| .__/|_| |_|_|\\___|_\n");
        printf("\t\t\t\t\t| \\ | |    | | | |                | |\n");
        printf("\t\t\t\t\t|  \\| | ___| |_|_|     _____  _ __| | _____\n");
        printf("\t\t\t\t\t| . ` |/ _ \\ __\\ \\ /\\ / / _ \\| '__| |/ / __|\n");
        printf("\t\t\t\t\t| |\\  |  __/ |_ \\ V  V / (_) | |  |   <\\__ \\\n");
        printf("\t\t\t\t\t|_| \\_|\\___|\\__| \\_/\\_/ \\___/|_|  |_|\\_\\___/\n\n");

        print_centered("Choisir un réseau trophique:\n\n");
        print_centered("1. Foret temperee");
        print_centered("2. Ocean Arctique");
        print_centered("3. Savane Africaine");
        print_centered("4. Quitter");
        center_cursor();
        fgets(choix, sizeof(choix), stdin);
        choix[strcspn(choix, "\n")] = '\0'; // Retirer le caractère de nouvelle ligne

        if (strcmp(choix, "Foret temperee") == 0) {
            afficher_loading("Loading");
            strcpy(nomfichier, "graphe1.txt");
        } else if (strcmp(choix, "Ocean Arctique") == 0) {
            afficher_loading("Loading");
            strcpy(nomfichier, "graphe2.txt");
        } else if (strcmp(choix, "Savane Africaine") == 0) {
            afficher_loading("Loading");
            strcpy(nomfichier, "graphe3.txt");
        } else if (strcmp(choix, "Quitter") == 0) {
            break;
        } else {
            afficher_loading("Erreur");
            continue;
        }

        charger_et_afficher_reseau(nomfichier, noeuds, arcs, &noeud_count, &arc_count);

        // Demander à l'utilisateur de sélectionner une espèce par son nom
        printf("\nEntrez le nom de l'espèce pour afficher les chaînes alimentaires dont elle dépend : ");
        scanf("%s", espece_nom);

        // Trouver l'ID correspondant au nom entré
        espece_id = -1;
        for (int i = 0; i < noeud_count; i++) {
            if (strcmp(noeuds[i].nom, espece_nom) == 0) {
                espece_id = noeuds[i].id;
                break;
            }
        }

        if (espece_id != -1) {
            afficher_graphe_ascendants(noeuds, arcs, noeud_count, arc_count, espece_id);
            afficher_niveaux_trophiques(noeuds, arcs, noeud_count, arc_count, espece_id);
        } else {
            printf("Espèce \"%s\" introuvable dans le réseau.\n", espece_nom);
        }

        // Demander à l'utilisateur de simuler la disparition d'une espèce
        printf("\nEntrez le nom de l'espèce à simuler pour disparition : ");
        scanf("%s", espece_nom);

        // Trouver l'ID correspondant à l'espèce
        espece_id = -1;
        for (int i = 0; i < noeud_count; i++) {
            if (strcmp(noeuds[i].nom, espece_nom) == 0) {
                espece_id = i;
                break;
            }
        }

        if (espece_id != -1) {
            simuler_disparition(noeuds, arcs, &arc_count, espece_id);
            calculer_degres(noeuds, arcs, noeud_count, arc_count); // Recalcul des degrés
            calculer_centralite_intermediarite(noeuds, arcs, noeud_count, arc_count); // Recalcul de la centralité
        } else {
            printf("Espèce \"%s\" introuvable.\n", espece_nom);
        }
        charger_reseau(nomfichier, noeuds, arcs, &noeud_count, &arc_count);
        simuler_evolution_population(noeuds, arcs, noeud_count, arc_count);
        system("cls");
        afficher_rapport(noeuds, arcs, noeud_count, arc_count);

        while (_getch() != 27); // 27 is the ASCII code for Esc
    }

    return 0;
}



