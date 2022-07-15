#include "node.h"

/** Declaration de mes variables static **/
float Node::time = (float)0.0;
float Node::vitesseParticule = 90;  /// Valeur à changer, vitesse relative au systeme
bool Node::minNearest = true;
bool Node::magNearest = true;
bool Node::modeTexture = true;
bool Node::modeFilDeFer = false;
bool Node::modeAfficherOrbite = true;
bool Node::modeAxes = true;
bool Node::modeXYZ = false;
bool Node::vaisseauEnMarche = false;
bool Node::marcheArriere = false;

Node::Node(Node* Parent, const char* Name)
{
    m_Parent = Parent;
    m_Name = Name;
}   // constructeur

Node::~Node(void)
{
    m_Parent = NULL;
    m_children.clear();
} // Destructeur

/*****************************************************************************/

void Node::UpdateDraw(void)
{
    if(!m_children.empty())
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            if(m_children[i] != NULL)
            {
                m_children[i]->UpdateDraw();
            }
        }
    }
} // Update()

/*****************************************************************************/

void Node::UpdateName(void)
{
    if(!m_children.empty())
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            if(m_children[i] != NULL)
            {
                m_children[i]->UpdateName();
            }
        }
    }
} // Update()

/*****************************************************************************/

Node* Node::GetParentNode(void) const
{
    return(m_Parent);
} // GetParentNode()

/*****************************************************************************/

void Node::SetParentNode(Node* NewParent)
{
    if(NULL != m_Parent)
    {
        m_Parent->RemoveChildNode(this);
    }
    m_Parent = NewParent;
} // setParentNode()

/*****************************************************************************/

void Node::AddChildNode(Node* ChildNode)
{
    if(ChildNode != NULL)
    {
        if(ChildNode->GetParentNode() != NULL)
        {
            ChildNode->SetParentNode(this);
        }
        m_children.push_back(ChildNode);
    }
} // AddChildNode()

/*****************************************************************************/

void Node::RemoveChildNode(Node* ChildNode)
{
    if(NULL != ChildNode && !m_children.empty())
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            if(m_children[i] == ChildNode)
            {
                m_children.erase(m_children.begin() + i);
                break; // break the for loop
            }
        }
    }
} // RemoveChildNode()

/*****************************************************************************/

const char* Node::GetNodeName(void) const
{
    return(m_Name);
} // GetNodeName()

/*****************************************************************************/

Node* Node::GetChildNodeByName(char* nameToFind, Node* Retval)
// va chercher récursivement le node correspondant
{
    if(!m_children.empty() && Retval==NULL)
    {
        for(size_t i = 0; i < m_children.size(); ++i)
        {
            if(0 == strcmp(m_children[i]->m_Name, nameToFind))
            {
                Retval = m_children[i];
                break; // break the for loop
            }
            else if (!m_children[i]->m_children.empty())
            {
                GetChildNodeByName(nameToFind,Retval);
            }
        }
    }
    return(Retval);
}

/*****************************************************************************/
/*****************************************************************************/

void Node::setTime(float newTime)
{
    time = newTime;
}

/*****************************************************************************/

void Node::setModeMinFilter(bool etat)
{
    minNearest = etat;
}

/*****************************************************************************/

void Node::setModeMagFilter(bool etat)
{
    magNearest = etat;
}

/*****************************************************************************/

void Node::setModeTexture(bool etat)
{
    modeTexture = etat;
}

/*****************************************************************************/

void Node::setModeFilDeFer(bool etat)
{
    modeFilDeFer = etat;
}

/*****************************************************************************/

void Node::setAfficherOrbite(bool etat)
{
    modeAfficherOrbite = etat;
}

/*****************************************************************************/

void Node::setAxes(bool etat)
{
    modeAxes = etat;
}

/*****************************************************************************/

void Node::setXYZ(bool etat)
{
    modeXYZ = etat;
}

/*****************************************************************************/

void Node::setVaisseauMarche(bool etat)
{
    vaisseauEnMarche = etat;
}

/*****************************************************************************/

void Node::setMarcheArriere()
{
    marcheArriere = !marcheArriere;
}
