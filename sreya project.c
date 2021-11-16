#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int ph,maxSize;
struct SG
{
    int value;
    struct SG *right,*left,*sibling,*parent;///////////
};

int max(int a,int b)
{
    if(a>=b)
    return a;
    else
    return b;
}

int size(struct SG *t)
{
    if(t==NULL)
      return 0;
    else 
      return (1+size(t->left)+size(t->right));
}

int height(struct SG *t)
{
    if(t==NULL)
      return 0;
    else 
      return max(size(t->left),size(t->right))+1;
}
struct SG* FindScapegoat(struct SG *t)
{
    int totalSize;
    int Size = 1;
    int height = 0;
    while (t->parent){
     height = height + 1;
     totalSize = 1 + Size + size(t->sibling);
     if(height > (log(totalSize))/(log(ph)))
       return t->parent;
    t= t->parent;
    Size=totalSize;
    }
}
struct SG* search(struct SG *t,int num)
{
    while(t!=NULL)
    {
        if(num==(t)->value)
        return t;
        else if (num<=(t)->left->value)
        return search((t)->left,num);
        else
        return search((t)->left,num);
    }
}

struct SG* FlattenTree(struct SG *t,struct SG *head)
{
    if(t==NULL)
      return head;
    t->right = FlattenTree(t->right,head);
    return FlattenTree(t->left,t);
}

struct SG* BuildHeightBalancedTree(int size, struct SG *head)
{
    struct SG *root,*last;
    if(size==1) 
    return head;
    else if(size==2) 
    {
       (head->right)->left=head;
       return head->right;
    }
    root = BuildHeightBalancedTree(((size-1)/2),head->right);
    last = BuildHeightBalancedTree(((size-1)/2),root->right);
    root->left = head;
    return last;

}
struct SG* RebuildTree(int size,struct SG *scapegoat)
{
    struct SG *head;
    head=FlattenTree(scapegoat,NULL);
    BuildHeightBalancedTree(size,head);
    while(head->parent!=NULL)
    {
        head = head->parent;
    }
    return head;
    
}

void insert(struct SG**t,int num)
{
    if(*t==NULL)
    { 
        t= (struct SG**)malloc(sizeof (struct SG));
        (*t)->left  = NULL;
        (*t)->value=num;
        (*t)->right = NULL;
        
    }    
    else{
        if(num<(*t)->value)
        insert(&((*t)->left),num);
        else
        insert(&((*t)->right),num);
    }
}

void Insert(struct SG**t,int num)
{
    double s;
    int h;
    struct SG *scapegoat, *node;
    insert(t,num);
    s=(log(size(*t)))/(log(ph));
    h=height(*t);
    if (h==-1 )
       printf("can not insert");
    else if( h > s)
    {
        node=search(*t,num);
        scapegoat=FindScapegoat(node);
        RebuildTree(size(*t), scapegoat);
    }
}


int locate ( struct SG *root, int num, struct SG **par,struct SG*x)
{
    struct SG*q;
    q = root; 
    *par = NULL;

    while ( q != NULL)
    {
        if ( q->value == num)
        {
            x = q;
            return 1; 
            
        }
        *par =q;
        if (q->value > num)
        q = q->left;
        else
        q = q->right;
    }
}

int del( struct SG **root, int num) 
{
    int found;
    struct SG *parent, *x, *xsucc;
    if ( *root == NULL)
    { 
        printf ("Tree is empty\n"); 
        return 0; 
        
    }
    parent = x = NULL;
    found=locate (*root, num, &parent, x );
    if ( found!=1)
    { 
        printf ("\nNode to be deleted not found\n");
        return 0;; 
        
    }
    if ( x->left != NULL && x->right != NULL)
    {

        parent = x; 
        xsucc = x->right ;

        while ( xsucc->left != NULL) 
        { 
            parent = xsucc;
            xsucc = xsucc->left;
        }

        x->value = xsucc->value ; 
        x = xsucc;
    }
    if ( x->left == NULL && x->right == NULL)
    {
        if ( parent->right == x) 
        parent->right = NULL; 
        else 
        parent->left = NULL;
        free (x); 
        return 1;
    }

    if ( x->left == NULL && x->right != NULL)
    {
        if ( parent->left == x) 
        parent->left = x->right; 
        else 
        parent->right = x->right;
        free ( x);
        return 1;
    }

    if ( x->left != NULL && x->right == NULL)
    {
        if ( parent->left == x) 
        parent->left = x->left; 
        else 
        parent->right = x->left;
        free (x); 
        return 1;
    }
}
void Delete(struct SG **root, int num)
{
    int k;
    k=del(root, num);
    if(k)
    {
      if (size(*root)< (ph*maxSize))
        RebuildTree(size(*root),*root);
    }
}
int main()
{
    struct SG *bt, *s;
    int d;
    char ch;
    while(1)
    { 
        printf("Enter a balance factor:");
        scanf("%d",&ph);
        printf("Enter a max size of the tree:");
        scanf("%d",&maxSize);
        printf("i) Insert :\ns) search :\np) delete:\nq) Quit:\nEnter char:");
        scanf("%s",&ch);
        switch(ch)
        {
            case 'i':
            printf("Enter a data to insert:");
            scanf("%d",&d);
            Insert(&bt,d);
            break;
            case 's':
            printf("Enter data you want to search:");
            scanf("%d",&d);
            s=search(bt,d);
            break;
            case 'd':
            printf("Enter data you want to delete:");
            scanf("%d",&d);
            Delete(&bt,d);
            break;
            case'q':
            exit(0);
        }
    }
}

