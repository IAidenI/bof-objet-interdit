# BOF : L'objet Interdit

Un petit jeu en **C/C++ avec raylib** pour découvrir les **buffer overflows** de façon ludique et pédagogique.

## 🎮 Concept
Vous incarnez un personnage pouvant parler à des PNJ, gérer un petit inventaire et tenter d’obtenir **L'Objet Interdit**.  
Le problème ? **L'Objet Interdit** est indispensable pour terminer le jeu, mais il semble impossible à récupérer par des moyens « normaux ».  
Saurez‑vous quand même finir le jeu ?

## 🚀 Compilation & Exécution
```bash
# Cloner le dépôt
git clone https://github.com/IAidenI/bof-objet-interdit
cd bof-objet-interdit

# Compiler
make clean && make

# Lancer le jeu
./bofgame
```

## 🎯 Objectif du joueur
- Explorer le jeu  
- Découvrir le bug  
- Terminer le jeu

## 📌 État du projet (TODO / DONE)
### ✅ Fait
- Prototype (PoC) en console fonctionnel : lecture du nom, remplissage d'inventaire, appel des fonctions `inventory().add()`, `inventory().hasEnoughOf()` et affichage d'informations.
- Définitions d'entités de base (`Player`, `Item`, `Inventory`, `Hitbox`, `Position`) et headers dans `headers/`.
- Main minimal démontrant la vulnérabilité.

### 🔜 À faire / Améliorations prévues
- Intégrer l'interface graphique avec **raylib**.
- Créer des assets : PNG pour le joueur et les PNJ, polices et icônes.
- Implémenter le système de dialogues avec choix et conséquences.
- Ajout d'un menu avancé montrant l'état des registres pour mieux comprendre ce qu'il se passe
- Compléter et stabiliser le système d'inventaire (UI, drag & drop si besoin).
- Documentation pédagogique dans `/docs` expliquant la faille et comment elle fonctionne.

## ⚠️ Avertissement
Ce projet contient volontairement une vulnérabilité à des fins **pédagogiques**. **Ne reproduisez pas** ces techniques sur des systèmes réels ou en production sans autorisation explicite. Utilisez ce projet uniquement pour apprendre et expérimenter dans un environnement contrôlé.
