#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>
#include "geometry.hpp"
#include "colors.hpp"
#include "style.hpp"

inline constexpr int NOTIFICATION_LIMIT = 4;
inline constexpr int NOTIFICATION_DURATION_MAX_SECONDS = 2;

// Convertit une ligne en plusieurs en fonction d'une taille maximal
vector<string> Multilines(TextStyle data, float maxWidth);

struct Notif {
    int id;
    string user;
    string content;
    long long start;

    bool isExpired(long long nowMs) const {
        long long diff = nowMs - start;
        return diff >= (NOTIFICATION_DURATION_MAX_SECONDS * 1000);
    }
};

class Notification {
    private:
        // Configuration
        Frame parentFrame;
        float maxWidth;
        const Font* font = nullptr;
        Padding padIn;
        Margins padOut;
        int notifCount = 0;
        
        // Données
        vector<Notif> notifications;

        bool isValid(); // Vérifie que la configuration à bien été faite
        int generateID(); // Génère un id unique pour chaque notifications
    public:
        void config(Frame parentFrame, float maxWidth, const Font& font, Padding padIn = DEFAULT_PAD_IN, Margins padOut = DEFAULT_PAD_OUT); // Configure les paramètres
        void push(string user, string message); // Ajoute une notification
        void update(); // Supprime de la list de notification si elle a expiré
        void draw(); // Dessine une barre de notification en bas à gauche de l'écran
        void debug();
};

#endif // NOTIFICATION_H
