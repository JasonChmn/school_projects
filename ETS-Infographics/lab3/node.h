#ifndef NODE_H
#define NODE_H
#include <vector>
#include <cstddef>
#include <stdio.h>
#include <cstring>
#include <iostream>

class Node
{
public:

    /****************** Source du site : ***************************
     * http://scriptionary.com/2009/02/17/simple-scene-graph-in-c/ *
     ***************************************************************/
        // Constructeur
        Node(Node* Parent = NULL, const char* Name =NULL);
        virtual ~Node();

        virtual void UpdateDraw(void);
        virtual void UpdateName(void);

        Node* GetParentNode(void) const;
        void SetParentNode(Node* NewParent);

        void AddChildNode(Node* ChildNode);
        void RemoveChildNode(Node* ChildNode);

        const char* GetNodeName(void) const;

        Node* GetChildNodeByName(char* nameToFind, Node* Retval);

        void setTime(float newTime);

        void setModeTexture(bool etat);
        void setModeFilDeFer(bool etat);

        void setAfficherOrbite(bool etat);

        void setModeMinFilter(bool etat);
        void setModeMagFilter(bool etat);

        void setAxes(bool etat);

        void setVaisseauMarche(bool etat);
        void setMarcheArriere();

        void setXYZ(bool etat);

    protected:
        Node* m_Parent;
        const char* m_Name;
        std::vector<Node*> m_children;

        static bool minNearest;
        static bool magNearest;

        static bool modeTexture;
        static bool modeFilDeFer;
        static bool modeAfficherOrbite;

        static bool modeAxes;

        static bool modeXYZ;

        static bool vaisseauEnMarche;
        static bool marcheArriere;

        static float time;

        static float vitesseParticule;
};



#endif // NODE_H
