#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

struct Matiere {
    string nom;
    vector<double> notes;
};

struct Etudiant {
    string nom;
    map<string, Matiere> matieres;
};

class GestionNotes {
private:
    map<string, Etudiant> etudiants;

public:
    // Ajouter un �tudiant avec ses mati�res et notes
    void ajouterEtudiant() {
        string nom, id, matiere;
        vector<string> matieres;
        double note;
        int nombreMatieres, nombreNotes;

        cout << "Entrez le nom de l'etudiant : ";
        getline(cin, nom);
        cout << "Entrez l'ID de l'etudiant : ";
        cin >> id;
        cin.ignore();

        cout << "Combien de matieres ? ";
        cin >> nombreMatieres;
        cin.ignore();
        for (int i = 0; i < nombreMatieres; i++) {
            cout << "Entrez le nom de la matiere " << i + 1 << " : ";
            getline(cin, matiere);
            matieres.push_back(matiere);
        }

        if (etudiants.find(id) != etudiants.end()) {
            cout << "L'identifiant existe deja." << endl;
            return;
        }

        Etudiant etudiant{nom};
        for (const auto& matiere : matieres) {
            etudiant.matieres[matiere] = Matiere{matiere};
        }
        etudiants[id] = etudiant;
        cout << "Etudiant ajoute avec succes." << endl;

        for (const auto& mat : matieres) {
            cout << "Combien de notes pour la matiere " << mat << " ? ";
            cin >> nombreNotes;
            for (int j = 0; j < nombreNotes; j++) {
                cout << "Entrez la note " << j + 1 << " pour la matiere " << mat << " : ";
                cin >> note;
                etudiants[id].matieres[mat].notes.push_back(note);
            }
        }

        // Afficher la liste des notes et la moyenne generale
        genererBulletin(id);
    }

    // Supprimer une note pour un �tudiant
    void supprimerNote() {
        string id, matiere;
        int index;

        cout << "Entrez l'ID de l'etudiant : ";
        cin >> id;
        cout << "Entrez le nom de la matiere : ";
        cin.ignore();
        getline(cin, matiere);
        cout << "Entrez l'index de la note � supprimer : ";
        cin >> index;

        if (etudiants.find(id) != etudiants.end() && etudiants[id].matieres.find(matiere) != etudiants[id].matieres.end()) {
            if (index < etudiants[id].matieres[matiere].notes.size()) {
                etudiants[id].matieres[matiere].notes.erase(etudiants[id].matieres[matiere].notes.begin() + index);
                cout << "Note supprimee avec succes." << endl;
            } else {
                cout << "Index de note invalide." << endl;
            }
        } else {
            cout << "Etudiant ou matiere introuvable." << endl;
        }
    }

    // Calculer la moyenne g�n�rale d'un �tudiant
    double calculerMoyenne(const string& id) {
        if (etudiants.find(id) != etudiants.end()) {
            double total = 0;
            int count = 0;
            for (const auto& matiere : etudiants[id].matieres) {
                for (double note : matiere.second.notes) {
                    total += note;
                    count++;
                }
            }
            return (count > 0) ? (total / count) : 0.0;
        } else {
            cout << "Etudiant introuvable." << endl;
            return 0.0;
        }
    }

    // G�n�rer un bulletin de notes
    void genererBulletin(const string& id) {
        if (etudiants.find(id) != etudiants.end()) {
            cout << "Bulletin de notes pour " << etudiants[id].nom << " (ID: " << id << ")" << endl;
            for (const auto& matiere : etudiants[id].matieres) {
                cout << "Matiere: " << matiere.first << endl;
                cout << "Notes: ";
                for (double note : matiere.second.notes) {
                    cout << note << " ";
                }
                cout << endl;
            }
            cout << "Moyenne generale: " << fixed << setprecision(2) << calculerMoyenne(id) << endl;
        } else {
            cout << "Etudiant introuvable." << endl;
        }
    }

    // Rechercher un �tudiant par nom ou identifiant et afficher son bulletin
    void rechercherEtudiant() {
        string critere;
        cout << "Entrez le nom ou l'ID de l'etudiant a rechercher : ";
        cin.ignore();
        getline(cin, critere);

        for (const auto& [id, etudiant] : etudiants) {
            if (id == critere || etudiant.nom == critere) {
                genererBulletin(id);
                return;
            }
        }
        cout << "Etudiant introuvable." << endl;
    }

    // Sauvegarder les donn�es dans un fichier
    void sauvegarder() {
        string nomFichier;
        cout << "Entrez le nom du fichier pour sauvegarder les donnees : ";
        cin.ignore();
        getline(cin, nomFichier);

        ofstream fichier(nomFichier);
        if (fichier.is_open()) {
            for (const auto& [id, etudiant] : etudiants) {
                fichier << id << "," << etudiant.nom << endl;
                for (const auto& matiere : etudiant.matieres) {
                    fichier << matiere.first;
                    for (double note : matiere.second.notes) {
                        fichier << "," << note;
                    }
                    fichier << endl;
                }
            }
            fichier.close();
            cout << "Donnees sauvegardees avec succes." << endl;
        } else {
            cout << "Erreur lors de l'ouverture du fichier." << endl;
        }
    }
};

void afficherMenu() {
    cout << "                       Menu " << endl;
    cout << "******************************************************" << endl;
    cout << "* 1. Ajouter un etudiant                             *" << endl;
    cout << "* 2. Supprimer une note pour un etudiant             *" << endl;
    cout << "* 3. Rechercher un etudiant et afficher son bulletin *" << endl;
    cout << "* 4. Sauvegarder les bulletins dans un fichier       *" << endl;
    cout << "* 5. Quitter                                         *" << endl;
    cout << "******************************************************" << endl;
    cout << "Choisissez une option : ";
}

int main() {
    GestionNotes gestion;
    int choix;

    do {
        afficherMenu();
        cin >> choix;
        cin.ignore();

        switch (choix) {
            case 1:
                gestion.ajouterEtudiant();
                break;

            case 2:
                gestion.supprimerNote();
                break;

            case 3:
                gestion.rechercherEtudiant();
                break;

            case 4:
                gestion.sauvegarder();
                break;

            case 5:
                cout << "Quitter le programme." << endl;
                break;

            default:
                cout << "Option invalide. Veuillez reessayer." << endl;
        }
    } while (choix != 5);

    return 0;
}
