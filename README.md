# VulkanEngine

## À propos
Ce projet est une implémentation simplifiée d'un moteur de rendu avec l'API Vulkan, conçu pour initier les débutants aux concepts fondamentaux de cette API graphique moderne. Il présente une application minimale qui initialise Vulkan et affiche un écran avec un fond animé dont la couleur change progressivement.

## Fonctionnalités
- Initialisation complète de Vulkan avec VkBootstrap
- Création et gestion d'une fenêtre avec GLFW
- Création d'une chaîne de permutation (swapchain)
- Synchronisation avec sémaphores et barrières
- Cycle de rendu basique

## Prérequis
- Connaissances en C++
- Carte graphique compatible Vulkan
- Compilateur C++ (GCC, Clang, MSVC)
- IDE (Visual Studio, VS Code, CLion)

## Installation
1. Installez le SDK Vulkan depuis [LunarG](https://www.lunarg.com/vulkan-sdk/)
2. Configurez les variables d'environnement:
   ```
   VULKAN_SDK=chemin/vers/votre/installation
   PATH=%PATH%;%VULKAN_SDK%\Bin
   ```
3. Installez GLFW (https://www.glfw.org/)
4. Vérifiez l'installation:
   ```
   vulkaninfo
   ```

## Structure du Projet
```
vulkan-project/
├── src/                  # Code source (main.cpp, VkBootstrap.cpp)
├── include/              # Fichiers d'en-tête (VkBootstrap.h, VkBootstrapDispatch.h)
├── build/                # Fichiers de compilation
└── assets/               # Ressources (textures, modèles)
```

## Architecture de l'application
L'application est structurée autour de la classe `VulkanEngine` qui encapsule toutes les fonctionnalités nécessaires :
- Initialisation de l'API Vulkan et création d'une instance
- Configuration des périphériques physiques et logiques
- Gestion des files d'attente de commandes (Command Queues)
- Mise en place du swapchain pour l'affichage
- Structures de synchronisation (sémaphores, barrières, clôtures)

## Commandes de Compilation
```bash
# Créer le répertoire de build
mkdir -p build && cd build

# Générer les fichiers de compilation avec CMake
cmake ..

# Compiler le projet
make

# Exécuter l'application
./VulkanEngine
```

## Fonctionnement du Pipeline de Rendu
1. Acquisition d'une image depuis le swapchain
2. Transition de l'image vers un layout adaptée au rendu
3. Exécution des commandes de dessin (clear color animation)
4. Transition de l'image vers un layout de présentation
5. Présentation de l'image à l'écran

## Ressources d'Apprentissage Recommandées
- [Vulkan-guide.com](https://vulkan-guide.com) - Guide complet pour les débutants
- [Vulkan Tutorial](https://vulkan-tutorial.com) - Tutoriel détaillé étape par étape
- [Khronos Vulkan Registry](https://www.khronos.org/registry/vulkan/) - Documentation officielle
- [Awesome Vulkan](https://github.com/vinjn/awesome-vulkan) - Collection de ressources

## Dépannage Courant
- **Erreur de validation des couches** : Vérifiez que les couches de validation sont correctement installées et activées.
- **Problèmes de compatibilité matérielle** : Assurez-vous que votre carte graphique supporte Vulkan 1.3 et que les pilotes sont à jour.
- **Erreurs de synchronisation** : Vérifiez la création et l'utilisation des sémaphores et des barrières.
- **Problèmes d'allocation de mémoire** : Assurez-vous que les ressources graphiques sont correctement allouées et libérées.

## Prochaines Étapes
- Ajout d'un pipeline graphique complet
- Implémentation de chargeurs de textures
- Support de modèles 3D
- Implémentation d'un système de shaders
- Ajout d'une interface utilisateur

## Contribution
Les suggestions d'amélioration et les contributions sont les bienvenues. Veuillez créer une issue ou une pull request pour toute proposition.

## Licence
Ce projet est distribué sous licence MIT. Consultez le fichier LICENSE pour plus d'informations.

---

Bonne exploration de Vulkan ! N'hésitez pas à consulter régulièrement la documentation officielle de Vulkan pour approfondir vos connaissances.
