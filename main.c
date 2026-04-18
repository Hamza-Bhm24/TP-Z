 /**--------------------------------------------------------**/
  /**       C o n v e r s i o n   Z vers C (Standard)        **/
  /**             Realisee par Pr D.E ZEGOUR                 **/
  /**             E S I - Alger                              **/
  /**             Copywrite 2014                             **/
  /**--------------------------------------------------------**/


  /****************DDeclaration Des ACTION*****************************/

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>

  typedef int bool ;
  typedef char * string255 ;

  #define True 1
  #define False 0

  /** Implementation **\: FICHIER **/

  /* Traitement des fichiers ouverts */

  struct _Noeud
    {
      FILE * Var_fich ;
      char * Nom_fich ;
      int Sauv_pos;
      struct _Noeud *Suiv ;
    } ;

  typedef struct _Noeud * _Ptr_Noeud;

  _Ptr_Noeud  _Pile_ouverts  = NULL;

  /* Teste si un fichier est ouvert */
  _Ptr_Noeud _Ouvert ( char * Fp)
    {
      _Ptr_Noeud P;
      bool Trouv ;
      P = _Pile_ouverts; Trouv = False ;
      while ((P != NULL) && ! Trouv )
        if ( strcmp(P->Nom_fich, Fp) == 0)
        Trouv = True;
        else P = P->Suiv;
      return P;
    }

  /* Ajouter un fichier ouvert */
  void _Empiler_ouvert ( char *Fp, FILE *Fl)
    {
      _Ptr_Noeud  P ;
      P = (_Ptr_Noeud) malloc( sizeof( struct _Noeud)) ;
      P->Nom_fich = Fp;
      P->Var_fich = Fl;
      P->Suiv = _Pile_ouverts;
      _Pile_ouverts = P;
    }

  /* Supprimer un fichier ouvert et rendre son nom*/
  char * _Depiler_ouvert ( FILE *Fl)
    {
      char * Fp = malloc (100);
      _Ptr_Noeud P,  Prec  ;
      P= _Pile_ouverts;
      Prec = NULL;
      while (P->Var_fich != Fl )
        { Prec = P ; P = P->Suiv ;}
      strcpy(Fp, P->Nom_fich);
      if (Prec != NULL)
        Prec->Suiv = P->Suiv;
      else _Pile_ouverts = P->Suiv;
      free (P);
      return Fp ;
    }


  /** Fichiers **/

  typedef char _Tx[255];
  typedef string255 Typestruct1_s;
  typedef _Tx Typestruct1_s_Buf;


  /** Machine abstaite sur les fichiers **/

  void Ouvrir_s ( FILE **s , char *Fp , char * Mode )
    {
      _Ptr_Noeud P = _Ouvert(Fp);
      if ( P != NULL )
      /* Le fichier est deja ouvert */
        {
         P->Sauv_pos = ftell (P->Var_fich);
         fclose(P->Var_fich);
        }
      /* Le fichier est non ouvert */
      if ( strcmp(Mode,"A") == 0)
        *s = fopen(Fp, "r+b");
      else
        *s = fopen(Fp, "w+b");
      _Empiler_ouvert( Fp, *s);
    }

  void Fermer_s ( FILE * s )
    {
      char * Fp = malloc(100);
      _Ptr_Noeud P;
      strcpy(Fp, _Depiler_ouvert(s));
      fclose(s) ;
      /* Ya-til un fichier ouvert avec le m?me nom ?  */
      /* Si Oui, le Reouvrir a la position sauvegardee */
      P =  _Ouvert (Fp);
      if ( P != NULL)
      {
        s = fopen(P->Nom_fich, "r+b");
        fseek(s, P->Sauv_pos, 0);
      }
    }

  void Ecrireseq_s ( FILE * s, Typestruct1_s Buf  )
    {
      Typestruct1_s_Buf Buffer ;
      int I, J;
     for(J=0; J<= strlen(Buf); ++J)
         Buffer[J] = Buf [J];
      fwrite(&Buffer, sizeof( Typestruct1_s_Buf), 1, s)  ;
    }

  void Ecriredir_s ( FILE * s, Typestruct1_s Buf, int N )
    {
      Typestruct1_s_Buf Buffer ;
      int I, J;
     for(J=0; J<= strlen(Buf); ++J)
         Buffer[J] = Buf [J];
      fseek(s, (long) (N-1)* sizeof(  Typestruct1_s_Buf), 0);
      fwrite(&Buffer, sizeof( Typestruct1_s_Buf), 1, s)  ;
    }

  void Lireseq_s ( FILE * s, Typestruct1_s Buf )
    {
      Typestruct1_s_Buf Buffer ;
      int I, J;
      if (fread(&Buffer, sizeof( Typestruct1_s_Buf), 1, s) != 0) {
      for(J=0; J<= strlen(Buffer); ++J)
        Buf [J] = Buffer[J]  ;
      }
    }

  void Liredir_s ( FILE * s, Typestruct1_s Buf, int N)
    {
      Typestruct1_s_Buf Buffer ;
      int I, J;
      fseek(s, (long) (N-1)* sizeof( Typestruct1_s_Buf), 0 );
      fread(&Buffer, sizeof( Typestruct1_s_Buf), 1, s);
      for(J=0; J<= strlen(Buffer); ++J)
        Buf [J] = Buffer[J]  ;
    }

  void Rajouter_s ( FILE * s, Typestruct1_s Buf )
    {
      Typestruct1_s_Buf Buffer ;
      int I, J;
     for(J=0; J<= strlen(Buf); ++J)
         Buffer[J] = Buf [J];
      fseek(s, 0, 2); /* Fin du fichier */
      fwrite(&Buffer, sizeof( Typestruct1_s_Buf), 1, s)  ;
    }

  bool Finfich_s (FILE * s)
    {
      long K = ftell(s);
      fseek(s, 0, 2); /* Fin du fichier */
      long K2 = ftell(s);   /* position a partir du debut */
      if  (K==K2)
        { fseek(s, K, 0); return 1;}
      else
        { fseek(s, K, 0); return 0;}
    }

  int Alloc_bloc_s (FILE * s)
    {
      long K;
      fseek(s, 0, 2); /* Fin du fichier */
      K = ftell(s);   /* position a partir du debut */
      K = K / sizeof (Typestruct1_s_Buf);
      K ++;
      return(K);
    }


  /** Implementation **\: ARBRE BINAIRE DE CHAINES DE CARACTERES**/

  /** Arbres de recherche binaire **/

  typedef string255 Typeelem_As   ;
  typedef struct Noeud_As * Pointeur_As ;

  struct Noeud_As
    {
      Typeelem_As  Val ;
      Pointeur_As Fg ;
      Pointeur_As Fd ;
      Pointeur_As Pere ;
     } ;

  Typeelem_As Info_As( Pointeur_As P )
    { return P->Val;   }

  Pointeur_As Fg_As( Pointeur_As P)
    { return P->Fg ; }

  Pointeur_As Fd_As( Pointeur_As P)
    { return P->Fd ; }

  Pointeur_As Pere_As( Pointeur_As P)
    { return P->Pere ; }

  void Aff_info_As ( Pointeur_As P, Typeelem_As Val)
    {
       strcpy( P->Val , Val );
    }

  void Aff_fg_As( Pointeur_As P, Pointeur_As Q)
    { P->Fg =  Q;  }

  void Aff_fd_As( Pointeur_As P, Pointeur_As Q)
    { P->Fd =  Q ; }

  void Aff_pere_As( Pointeur_As P, Pointeur_As Q)
    { P->Pere =  Q ; }

  void Creernoeud_As( Pointeur_As *P)
    {
      *P = (struct Noeud_As *) malloc( sizeof( struct Noeud_As))   ;
      (*P)->Val = malloc(255 * sizeof(string255));
      (*P)->Fg = NULL;
      (*P)->Fd = NULL;
      (*P)->Pere = NULL;
    }

  void Liberernoeud_As( Pointeur_As P)
    { free( P ) ; }

  /** Variables du programme principal **/
  FILE *F;
  Typestruct1_s S ;
  Pointeur_As Bst0=NULL;
  Pointeur_As Bst1=NULL;
  Pointeur_As Bst2=NULL;
  Pointeur_As Bst3=NULL;
  int Choix;
  int Bst;
  string255 Val;
  bool For_count;
  int X;
  int Z;
  int Y;

  /** Fonctions standards **/

  int Aleanombre( int N )
    { return ( rand() % N ); }

  char  *Aleachaine ( int N )
    {
      int k;
      char  * Chaine = malloc(N+1);

      char Chr1[26] = "abcdefghijklmnopqrstuvwxyz";
      char Chr2[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

      for (k=0;k<N; k++)
        switch ( rand() % 2 ){
        case 0 :  *(Chaine+k) = Chr1[rand() % 26] ; break ;
        case 1 :  *(Chaine+k) = Chr2[rand() % 26] ; break ;
        }
      Chaine[k] =  '\0' ;

      return (Chaine);
    }

  char *Caract ( string255 Ch , int I )
    {
      char *s = malloc(2);
      s[0] = Ch[I-1];
      s[1] = '\0';
      return  s;
    }

  int Max (int a, int b)
    {
      if (a > b) return(a);
      else return(b);
    }


  /** Prototypes des fonctions **/

  void Generateandreadfile (FILE *F);
  void Recherche (Pointeur_As *R , Pointeur_As *P , Pointeur_As *Parent , string255 *Val , int *Path);
  void Insertion (Pointeur_As *R , string255 *Val , Pointeur_As *Q , int *Path);
  void Build_bst (Pointeur_As *R , char *filename1 , int *Bst);
  void Right_rotation (Pointeur_As *R , Pointeur_As *Q);
  void Left_rotation (Pointeur_As *R , Pointeur_As *Q);
  void Fix_bst3 (Pointeur_As *R , Pointeur_As *Q);
  void Fix_bst1 (Pointeur_As *R , Pointeur_As *Q);
  void Fix_bst2 (Pointeur_As *R , Pointeur_As *Q , int *Path);
  void Inorder (Pointeur_As *R , bool *For_count , int *X , int *Y , int *Z);
  void Free_tree (Pointeur_As *R);
  int  Depth (Pointeur_As *R) ;
  int  Maximum (int *A , int *B) ;
  void Word_search (Pointeur_As *Bst1 , Pointeur_As *Bst2 , Pointeur_As *Bst3 , string255 *Val);
  void Search_in_bst1 (Pointeur_As *Bst1 , string255 *Val , int *Path , bool *Trouv);
  void Search_in_bst2 (Pointeur_As *Bst2 , string255 *Val , int *Path , bool *Trouv);
  void Search_in_bst3 (Pointeur_As *Bst3 , string255 *Val , int *Path , bool *Trouv);
  void Count (Pointeur_As *R);
  void Inorder_rang (Pointeur_As *R , string255 *Word1 , string255 *Word2);
  void Rang_search (Pointeur_As *Bst1 , Pointeur_As *Bst2 , Pointeur_As *Bst3);
  void Swap_words (string255 *Word1 , string255 *Word2);
  void Words_in_one_level (Pointeur_As *R , int *Level , int *Counter , int *L);
  void Count_by_level (Pointeur_As *R);

  /*********************************************************/
  /**********************FFonction pour générer le Fichier des mots*******************************/
  void Generateandreadfile (FILE *F)
    {
      /** Variables locales **/
      Typestruct1_s S ;
      int I;
      int J;
      string255 _Sx;

      /** Corps du module **/
     S = malloc(255 * sizeof(char));
     _Sx = malloc(255 * sizeof(char));
     Ouvrir_s (&F , "F2.z" , "N" ) ;
     for( I  =  1 ;I <=  100 ; ++I){
       J  =  Aleanombre(7 ) ;
       if( J == 0) {
         strcpy (_Sx, "Y");
         strcpy (S,  strcat(_Sx, Aleachaine(Aleanombre(5 ) + 3 ))) ;

       } ;
       if( J == 1) {
         strcpy (_Sx, "Z");
         strcpy (S,  strcat(_Sx, Aleachaine(Aleanombre(5 ) + 3 ))) ;

       } ;
       if( J == 2) {
         strcpy (_Sx, "a");
         strcpy (S,  strcat(_Sx, Aleachaine(Aleanombre(5 ) + 3 ))) ;
         }
       else
         {
         strcpy (S,   Aleachaine(Aleanombre(5 ) + 3 )) ;

       } ;
       Ecrireseq_s ( F , S );
     } ;
     Fermer_s ( F ) ;
     Ouvrir_s (&F , "F2.z" , "A" ) ;
     I  =  0 ;
     while( ! Finfich_s ( F )) {
       Lireseq_s ( F , S ) ;
       printf ( " %s\n", S ) ;
       I  =  I + 1;
 } ;
     Fermer_s ( F ) ;

    }
  /*********************************************************************/
  /*****PProcedure qui recherche un noeud "p" de valeur "val" et son pere "parent" et calculer le path de recherche*****/
  void Recherche (Pointeur_As *R , Pointeur_As *P , Pointeur_As *Parent , string255 *Val , int *Path)
    {
      /** Variables locales **/
      bool Trouv;

      /** Corps du module **/
     *Path  =  0 ;
     Trouv  =  False ;
     *P  =  *R ;
     *Parent  =  NULL ;
     while( ( *P != NULL ) && ( ! Trouv )) {
       if( (strcmp( Info_As ( *P ), *Val) == 0  )) {
         Trouv  =  True ;
         }
       else
         {
         *Parent  =  *P ;
         if (strcmp( Info_As ( *Parent ),(*Val)) > 0  ) {
           *P  =  Fg_As ( *P ) ;
           *Path  =  *Path + 1 ;
           }
         else
           {
           *P  =  Fd_As ( *P ) ;
           *Path  =  *Path + 1 ;

         } ;

       } ;

 } ;

    }
  /*****************************************************************************/
  /*********************PProcedure pour insérer un noeud dans une BST************************************/
  void Insertion (Pointeur_As *R , string255 *Val , Pointeur_As *Q , int *Path)
    {
      /** Variables locales **/
      Pointeur_As P=NULL;
      Pointeur_As Parent=NULL;
      bool Trouv;

      /** Corps du module **/
     Recherche ( & *R , & P , & Parent , & *Val , & *Path ) ;
     if( ( P == NULL )) {
       Creernoeud_As (& *Q ) ;
       Aff_info_As ( *Q , *Val ) ;
       if( ( Parent == NULL )) {
         *R  =  *Q ;
         Aff_pere_As ( *R , NULL ) ;
         }
       else
         {
         if( (strcmp(Info_As (Parent),(*Val)) > 0  )) {
           Aff_fg_As ( Parent , *Q ) ;
           Aff_pere_As ( *Q , Parent ) ;
           }
         else
           {
           Aff_fd_As ( Parent , *Q ) ;
           Aff_pere_As ( *Q , Parent ) ;

         } ;

       } ;

     } ;

    }
  /*****************************************************************************/
  /**********************PProcedure pour construire BST0 BST1 BST2 BST3*******************************/

void Build_bst(Pointeur_As *R, char *filename, int *Bst) {
    /** Variables locales **/
    Pointeur_As Q = NULL;
    Typestruct1_s S;
    FILE *F;
    int I;
    int Path;
    /** Corps du module **/
    S = malloc(255 * sizeof(char));
    Ouvrir_s(&F, filename, "A");  // Use the provided filename, not hardcoded "F2.z"
    I = 0;
    while (!Finfich_s(F)) {
        Lireseq_s(F, S);
        Path = 0;
        Insertion(&*R, &S, &Q, &Path);
        if ((*Bst == 1)) {
            Fix_bst1(&*R, &Q);
        }
        if ((*Bst == 2)) {
            Fix_bst2(&*R, &Q, &Path);
        }
        if ((*Bst == 3)) {
            Fix_bst3(&*R, &Q);
        }
        I = I + 1;
    }
    Fermer_s(F);
    free(S);
}
  /****************************************************************************/
  /********************PProcedure qui fait la retation droite*****************************/
  void Right_rotation (Pointeur_As *R , Pointeur_As *Q)
    {
      /** Variables locales **/
      Pointeur_As Parent=NULL;
      Pointeur_As P=NULL;

      /** Corps du module **/
     Parent  =  Pere_As ( *Q ) ;
     P  =  Fd_As ( *Q ) ;
     Aff_fd_As ( *Q , Parent ) ;
     Aff_fg_As ( Parent , P ) ;
     Aff_pere_As ( *Q , Pere_As ( Parent ) ) ;
     if( ( Pere_As ( *Q ) != NULL )) {
       if( ( Parent == Fg_As ( Pere_As ( *Q ) ) )) {
         Aff_fg_As ( Pere_As ( Parent ) , *Q ) ;
         }
       else
         {
         Aff_fd_As ( Pere_As ( Parent ) , *Q ) ;

       } ;
       }
     else
       {
       *R  =  *Q ;

     } ;
     if( ( P != NULL )) {
       Aff_pere_As ( P , Parent ) ;

     } ;
     Aff_pere_As ( Parent , *Q ) ;

    }
  /****************************************************************************/
  /****************PProcedure qui fait la retation gauche*****************************/
  void Left_rotation (Pointeur_As *R , Pointeur_As *Q)
    {
      /** Variables locales **/
      Pointeur_As Parent=NULL;
      Pointeur_As P=NULL;

      /** Corps du module **/
     Parent  =  Pere_As ( *Q ) ;
     P  =  Fg_As ( *Q ) ;
     Aff_fg_As ( *Q , Parent ) ;
     Aff_fd_As ( Parent , P ) ;
     Aff_pere_As ( *Q , Pere_As ( Parent ) ) ;
     if( ( Pere_As ( *Q ) != NULL )) {
       if( ( Parent == Fg_As ( Pere_As ( *Q ) ) )) {
         Aff_fg_As ( Pere_As ( Parent ) , *Q ) ;
         }
       else
         {
         Aff_fd_As ( Pere_As ( Parent ) , *Q ) ;

       } ;
       }
     else
       {
       *R  =  *Q ;

     } ;
     if( ( P != NULL )) {
       Aff_pere_As ( P , Parent ) ;

     } ;
     Aff_pere_As ( Parent , *Q ) ;

    }
  /**************************************************************************/
  /***********************PProcedure pour régler BST3*************************/
  void Fix_bst3 (Pointeur_As *R , Pointeur_As *Q)
    {

      /** Corps du module **/
     if( (strcmp( Caract ( Info_As ( *Q ) , 1 ), "x") != 0  ) && (strcmp( Caract ( Info_As ( *Q ) , 1 ), "Y") != 0  ) && (strcmp( Caract ( Info_As ( *Q ) , 1 ), "Z") != 0  )) {
       while( ( Pere_As ( *Q ) != NULL )) {
         if( ( *Q == Fg_As ( Pere_As ( *Q ) ) )) {
           Right_rotation ( & *R , & *Q ) ;
           }
         else
           {
           if( ( *Q == Fd_As ( Pere_As ( *Q ) ) )) {
             Left_rotation ( & *R , & *Q ) ;

           } ;

         } ;

 } ;

     } ;

    }
  /***************************************************************************/
  /**********************PProcedure pour régler BST1***************************/
  void Fix_bst1 (Pointeur_As *R , Pointeur_As *Q)
    {
      /** Variables locales **/
      int X;
      int Z;
      int Y;

      /** Corps du module **/
     if( ( (strcmp( Caract ( Info_As ( *Q ) , 1 ), "x") == 0  ) || (strcmp( Caract ( Info_As ( *Q ) , 1 ), "Y") == 0  ) || (strcmp( Caract ( Info_As ( *Q ) , 1 ), "Z") == 0  ) )) {
       while( ( Pere_As ( *Q ) != NULL )) {
         if( ( *Q == Fg_As ( Pere_As ( *Q ) ) )) {
           Right_rotation ( & *R , & *Q ) ;
           }
         else
           {
           if( ( *Q == Fd_As ( Pere_As ( *Q ) ) )) {
             Left_rotation ( & *R , & *Q ) ;

           } ;

         } ;

 } ;

     } ;

    }
  /****************************************************************************/
  /********************PProcedure pour régler BST2********************************/
  void Fix_bst2 (Pointeur_As *R , Pointeur_As *Q , int *Path)
    {
      /** Variables locales **/
      int I;

      /** Corps du module **/
     I  =  0 ;
     if( ( (strcmp( Caract ( Info_As ( *Q ) , 1 ), "x") > 0  ) || (strcmp( Caract ( Info_As ( *Q ) , 1 ), "Y") > 0  ) || (strcmp( Caract ( Info_As ( *Q ) , 1 ), "Z") > 0  ) )) {
       while( ( Pere_As ( *Q ) != NULL )) {
         if( ( *Q == Fg_As ( Pere_As ( *Q ) ) )) {
           Right_rotation ( & *R , & *Q ) ;
           }
         else
           {
           if( ( *Q == Fd_As ( Pere_As ( *Q ) ) )) {
             Left_rotation ( & *R , & *Q ) ;

           } ;

         } ;
         I  =  I + 1 ;

 } ;

     } ;

    }
  /****************************************************************************/
  /*******************PProcedure de parcours inorder********************************/
  void Inorder (Pointeur_As *R , bool *For_count , int *X , int *Y , int *Z)
    {
      /** Variables locales **/
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     if( ( *R != NULL )) {
       _Px1 =  Fg_As ( *R ) ;
       Inorder ( &_Px1, & *For_count , & *X , & *Y , & *Z ) ;
       if( ( *For_count == True )) {
         if( (strcmp( Caract ( Info_As ( *R ) , 1 ), "x") == 0  )) {
           *X  =  *X + 1 ;
           printf ( " %s\n", Info_As(*R) ) ;

         } ;
         if( (strcmp( Caract ( Info_As ( *R ) , 1 ), "Z") == 0  )) {
           *Z  =  *Z + 1 ;
           printf ( " %s\n", Info_As(*R) ) ;

         } ;
         if( (strcmp( Caract ( Info_As ( *R ) , 1 ), "Y") == 0  )) {
           *Y  =  *Y + 1 ;
           printf ( " %s\n", Info_As(*R) ) ;

         } ;
         }
       else
         {
         printf ( " %s\n", Info_As(*R) ) ;

       } ;
       _Px2 =  Fd_As ( *R ) ;
       Inorder ( &_Px2, & *For_count , & *X , & *Y , & *Z ) ;

     } ;

    }
  /***************************************************************************/
  /****************PProcedure pour libérer une arbre****************************/
  void Free_tree (Pointeur_As *R)
    {
      /** Variables locales **/
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;
      Pointeur_As _Px3=NULL;
      Pointeur_As _Px4=NULL;

      /** Corps du module **/
     if( ( *R != NULL )) {
       if( ( ( Fd_As ( *R ) == NULL ) && ( Fg_As ( *R ) == NULL ) )) {
         Liberernoeud_As ( *R ) ;
         }
       else
         {
         if( ( ( Fd_As ( *R ) != NULL ) && ( Fg_As ( *R ) != NULL ) )) {
           _Px1 =  Fg_As ( *R ) ;
           Free_tree ( &_Px1) ;
           _Px2 =  Fd_As ( *R ) ;
           Free_tree ( &_Px2) ;
           Liberernoeud_As ( *R ) ;
           }
         else
           {
           if( ( Fg_As ( *R ) != NULL )) {
             _Px3 =  Fg_As ( *R ) ;
             Free_tree ( &_Px3) ;
             Liberernoeud_As ( *R ) ;
             }
           else
             {
             _Px4 =  Fd_As ( *R ) ;
             Free_tree ( &_Px4) ;
             Liberernoeud_As ( *R ) ;

           }
         }
       } ;
       *R  =  NULL ;

     }
    }
  /**************************************************************************/
  /********************FFonction pour calculer la profendeur d'une arbre*************************/
  int  Depth (Pointeur_As *R)
    {
      /** Variables locales **/
      int  Depth2 ;
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     if( ( *R == NULL )) {
       Depth2  =  - 1 ;
       }
     else
       {
       if( ( Fg_As ( *R ) == NULL ) && ( Fd_As ( *R ) == NULL )) {
         Depth2  =  0 ;
         }
       else
         {
         _Px1 =  Fd_As ( *R ) ;
         _Px2 =  Fg_As ( *R ) ;
         Depth2  =  1 + Max ( Depth ( &_Px1) , Depth ( &_Px2) );
       }
     }
     return Depth2 ;
    }
  /***********************************************************************/
  /**********************************************************************/
  int  Maximum (int *A , int *B)
    {
      /** Variables locales **/
      int  Maximum2 ;

      /** Corps du module **/
     if( ( *A > *B )) {
       Maximum2  =  *A ;
       }
     else
       {
       Maximum2  =  *B ;

     } ;

     return Maximum2 ;
    }
  /********************************************************************/
  /**************PProcedure pour chercher dans les BST*****************/
  void Word_search (Pointeur_As *Bst1 , Pointeur_As *Bst2 , Pointeur_As *Bst3 , string255 *Val)
    {
      /** Variables locales **/
      bool Trouv;
      Pointeur_As P=NULL;
      Pointeur_As Parent=NULL;
      int Path;
      int Tree_nb;

      /** Corps du module **/
     Trouv  =  False ;
     if( ( (strcmp( Caract ( *Val , 1 ), "x") == 0  ) || (strcmp( Caract ( *Val , 1 ), "Y") == 0  ) || (strcmp( Caract ( *Val , 1 ), "Z") == 0  ) )) {
       Tree_nb  =  1 ;
       Search_in_bst1 ( & *Bst1 , & *Val , & Path , & Trouv ) ;
       }
     else
       {
       if( ( (strcmp( Caract ( *Val , 1 ), "x") > 0  ) || (strcmp( Caract ( *Val , 1 ), "Y") > 0  ) || (strcmp( Caract ( *Val , 1 ), "Z") > 0  ) )) {
         Tree_nb  =  2 ;
         Search_in_bst2 ( & *Bst2 , & *Val , & Path , & Trouv ) ;
         }
       else
         {
         Tree_nb  =  3 ;
         Search_in_bst3 ( & *Bst3 , & *Val , & Path , & Trouv ) ;

       } ;

     } ;
     if( ( Trouv == True )) {
       printf ( " %s", "Word found in level" );
       printf ( " %d", Path );
       printf ( " %s", "of BST" );
       printf ( " %d", Tree_nb ) ;
       }
     else
       {
       printf ( " %s", "word not found\n" );

     } ;


    }
  /**************************************************************************/
  /**************PProcedure pour chercher dans la BST1******************/
  void Search_in_bst1 (Pointeur_As *Bst1 , string255 *Val , int *Path , bool *Trouv)
    {
      /** Variables locales **/
      Pointeur_As P=NULL;
      bool Stop;

      /** Corps du module **/
     *Path  =  0 ;
     *Trouv  =  False ;
     Stop  =  False ;
     P  =  *Bst1 ;
     while( ( P != NULL ) && ( ! *Trouv ) && ( ! Stop )) {
       if( (strcmp( Caract ( Info_As ( P ) , 1 ), "x") != 0  ) && (strcmp( Caract ( Info_As ( P ) , 1 ), "Y") != 0  ) && (strcmp( Caract ( Info_As ( P ) , 1 ), "Z") != 0  )) {
         Stop  =  True ;
         }
       else
         {
         if( (strcmp( Info_As ( P ), *Val) == 0  )) {
           *Trouv  =  True ;
           }
         else
           {
           if( (strcmp(Info_As ( P ),( *Val)) >= 0  )) {
             P  =  Fg_As ( P ) ;
             *Path  =  *Path + 1 ;
             }
           else
             {
             P  =  Fd_As ( P ) ;
             *Path  =  *Path + 1 ;

           } ;

         } ;

       } ;

 } ;

    }
  /**************************************************************************/
  /**************PProcedure pour chercher dans les BST2********************/
  void Search_in_bst2 (Pointeur_As *Bst2 , string255 *Val , int *Path , bool *Trouv)
    {
      /** Variables locales **/
      Pointeur_As P=NULL;

      /** Corps du module **/
     *Path  =  0 ;
     *Trouv  =  False ;
     P  =  *Bst2 ;
     while( ( P != NULL ) && ( ! *Trouv )) {
       if( (strcmp( Info_As ( P ), *Val) == 0  )) {
         *Trouv  =  True ;
         }
       else
         {
         if (strcmp(Info_As ( P ),(*Val)) > 0  ) {
           P  =  Fg_As ( P ) ;
           *Path  =  *Path + 1 ;
           }
         else
           {
           P  =  Fd_As ( P ) ;
           *Path  =  *Path + 1 ;

         } ;

       } ;

 } ;

    }
  /**************************************************************************/
  /***************PProcedure pour chercher dans les BST3********************/
  void Search_in_bst3 (Pointeur_As *Bst3 , string255 *Val , int *Path , bool *Trouv)
    {
      /** Variables locales **/
      Pointeur_As P=NULL;

      /** Corps du module **/
     *Path  =  0 ;
     *Trouv  =  False ;
     P  =  *Bst3 ;
     while( ( P != NULL ) && ( ! *Trouv )) {
       if( (strcmp( Info_As ( P ), *Val) == 0  )) {
         *Trouv  =  True ;
         }
       else
         {
         if (strcmp(Info_As ( P ) ,( *Val)) > 0 ) {
           P  =  Fg_As ( P ) ;
           *Path  =  *Path + 1 ;
           }
         else
           {
           P  =  Fd_As ( P ) ;
           *Path  =  *Path + 1 ;

         } ;

       } ;

 } ;

    }
  /***************************************************************/
  /**************PProcedure pour calculer le nombre des mots avec x Y Z dans une arbre********************/
  void Count (Pointeur_As *R)
    {
      /** Variables locales **/
      int X;
      int Z;
      int Y;
      bool For_count;

      /** Corps du module **/
     For_count  =  True ;
     X  =  0 ;
     Y  =  0 ;
     Z  =  0 ;
     Inorder ( & *R , & For_count , & X , & Y , & Z ) ;
     printf ( " %s", "Words starting with x:" );
     printf ( " %d", X ) ;
     printf ( " %s", "Words starting with Z:" );
     printf ( " %d", Z ) ;
     printf ( " %s", "Words starting with Y:" );
     printf ( " %d", Y ) ;


    }
  /*************************************************************/
  /****************PProcedure inoredr pour le rang search****************/
  void Inorder_rang (Pointeur_As *R , string255 *Word1 , string255 *Word2)
    {
      /** Variables locales **/
      Pointeur_As _Px1=NULL;
      Pointeur_As _Px2=NULL;

      /** Corps du module **/
     if( ( *R != NULL ))
     {

       if( (strcmp( Info_As ( *R ), *Word1) > 0  )) {
         _Px1 =  Fg_As ( *R ) ;
         Inorder_rang ( &_Px1, & *Word1 , & *Word2) ;

       } ;
       if( (strcmp( Info_As ( *R ), *Word1) >= 0  ) && (strcmp( Info_As ( *R ), *Word2) <= 0  )) {
         printf ( " %s", Info_As(*R) ) ;

       } ;
       if( (strcmp( Info_As ( *R ), *Word2) < 0  )) {
         _Px2 =  Fd_As ( *R ) ;
         Inorder_rang ( &_Px2, & *Word1 , & *Word2 ) ;

       } ;

     } ;


    }
  /************************************************************/
  /*****************PProcedure de rang search**********************/
  void Rang_search (Pointeur_As *Bst1 , Pointeur_As *Bst2 , Pointeur_As *Bst3)
    {
      /** Variables locales **/
      string255 Word1;
      string255 Word2;

      /** Corps du module **/
     Word1 = malloc(255 * sizeof(char));
     Word2 = malloc(255 * sizeof(char));
     printf ( " %s", "Enter Word1 : " ) ;
     scanf ( " %[^\n]", Word1 ) ;
     printf ( " %s", "Enter word2 : " ) ;
     scanf ( " %[^\n]", Word2 ) ;
     if( (strcmp( Word1, Word2) > 0  )) {
       Swap_words ( & Word1 , & Word2 ) ;

     } ;
     printf ( "The words in the rang are:" ) ;
     if( (strcmp( Caract ( Word1 , 1 ), "x") == 0  ) || (strcmp( Caract ( Word1 , 1 ), "Y") == 0  ) || (strcmp( Caract ( Word1 , 1 ), "Z") == 0  )) {
       Inorder_rang ( & *Bst1 , & Word1 , & Word2) ;
       }
     else
       {
       if( ( (strcmp( Caract ( Word1 , 1 ), "x") > 0  ) || (strcmp( Caract ( Word1 , 1 ), "Y") > 0  ) || (strcmp( Caract ( Word1 , 1 ), "Z") > 0  ) )) {
         Inorder_rang ( & *Bst2 , & Word1 , & Word2) ;
         }
       else
         {
         Inorder_rang ( & *Bst3 , & Word1 , & Word2) ;

       } ;

     } ;

    }
  /***********************************************************************/
  /******************PProcedure pour swaper word1 et word2**************************/
  void Swap_words (string255 *Word1 , string255 *Word2)
    {
      /** Variables locales **/
      string255 Save;

      /** Corps du module **/
     Save = malloc(255 * sizeof(char));
     strcpy (Save,   *Word1) ;
     strcpy (*Word1,   *Word2) ;
     strcpy (*Word2,   Save) ;

    }
  /************************************************************/
  /*********FFonction pour calculer le nombre de x Y Z Dans un seul niveau***************/
  void Words_in_one_level (Pointeur_As *R , int *Level , int *Counter , int *L)
    {
      /** Variables locales **/
      Pointeur_As _Px1=NULL;
      int _Px2;
      Pointeur_As _Px3=NULL;
      int _Px4;

      /** Corps du module **/
     if( ( *R != NULL )) {
       if( ( *Level == *L )) {
         if( (strcmp( Caract ( Info_As ( *R ) , 1 ), "x") == 0  ) || (strcmp( Caract ( Info_As ( *R ) , 1 ), "Y") == 0  ) || (strcmp( Caract ( Info_As ( *R ) , 1 ), "Z") == 0  )) {
           *Counter  =  *Counter + 1 ;

         } ;

       } ;
       _Px1 =  Fg_As ( *R ) ;
       _Px2 =  ( *Level + 1 ) ;
       Words_in_one_level ( &_Px1, &_Px2, & *Counter , & *L ) ;
       _Px3 =  Fd_As ( *R ) ;
       _Px4 =  ( *Level + 1 ) ;
       Words_in_one_level ( &_Px3, &_Px4, & *Counter , & *L ) ;

     } ;

    }
  /***********************************************************/
  /*********************FFonction pour calculer le nombre de x Y Z dans toutes l'arbre****************************/
  void Count_by_level (Pointeur_As *R)
    {
      /** Variables locales **/
      int L;
      int Counter;
      int Level;

      /** Corps du module **/
     for( L  =  0 ;L <=  ( Depth ( & *R ) ) ; ++L){
       Counter  =  0 ;
       Level  =  0 ;
       Words_in_one_level ( & *R , & Level , & Counter , & L ) ;
       printf ( "the number of nodes starting with x Y Z in level %d is %d \n",L,Counter);


     } ;


    }


void print_header(char* title) {
    printf("\n");
    printf("================================================================================\n");
    printf("                              %s\n", title);
    printf("================================================================================\n");
}

void print_separator() {
    printf("--------------------------------------------------------------------------------\n");
}

void print_tree_info(char* tree_name, Pointeur_As root) {
    printf("\n%s Information:\n", tree_name);
    printf("- Depth: %d\n", Depth(&root));
    printf("- Root: %s\n", root ? Info_As(root) : "NULL");
    print_separator();
}

void inorder_display(Pointeur_As *R, int level) {
    if (*R != NULL) {
        Pointeur_As left = Fg_As(*R);
        inorder_display(&left, level + 1);

        for(int i = 0; i < level; i++) printf("  ");
        printf("|___ %s (Level %d)\n", Info_As(*R), level);

        Pointeur_As right = Fd_As(*R);
        inorder_display(&right, level + 1);
    }
}

void clearScreen()
{
    printf("\033[H\033[J"); // ANSI escape code to clear screen
}


typedef struct {
    int found_words_bst0;
    int unfound_words_bst0;
    int found_words_triplet;
    int unfound_words_triplet;
    int path_length_found_bst0;
    int path_length_unfound_bst0;
    int path_length_found_triplet;
    int path_length_unfound_triplet;
} SearchResults;

// Generate file with N random words
void generate_test_file(char* filename, int N) {
    FILE *F;
    Typestruct1_s S = malloc(255 * sizeof(char));
    string255 _Sx = malloc(255 * sizeof(char));

    Ouvrir_s(&F, filename, "N");

    for (int i = 1; i <= N; i++) {
        int J = Aleanombre(10);
        if (J == 0) {
            strcpy(_Sx, "Y");
            strcpy(S, strcat(_Sx, Aleachaine(Aleanombre(5) + 3)));
        } else if (J == 1) {
            strcpy(_Sx, "Z");
            strcpy(S, strcat(_Sx, Aleachaine(Aleanombre(5) + 3)));
        } else if (J == 2) {
            strcpy(_Sx, "a");
            strcpy(S, strcat(_Sx, Aleachaine(Aleanombre(5) + 3)));
        } else {
            strcpy(S, Aleachaine(Aleanombre(5) + 3));
        }
        Ecrireseq_s(F, S);
    }
    Fermer_s(F);

    free(S);
    free(_Sx);
}

void generate_word_pairs_file(char* filename, int num_pairs) {
    FILE *F;
    Typestruct1_s S = malloc(255 * sizeof(char));

    Ouvrir_s(&F, filename, "N");

    for (int i = 0; i < num_pairs; i++) {
        string255 word1 = Aleachaine(Aleanombre(5) + 3);
        string255 word2 = Aleachaine(Aleanombre(5) + 3);

        // Ensure word1 <= word2
        if (strcmp(word1, word2) > 0) {
            string255 temp = word1;
            word1 = word2;
            word2 = temp;
        }

        // Store as "word1,word2"
        sprintf(S, "%s,%s", word1, word2);
        Ecrireseq_s(F, S);
    }

    Fermer_s(F);
    free(S);
}

//  search function for BST0 that returns path length
int search_bst0_with_path(Pointeur_As *R, string255 *Val, bool *found) {
    int path = 0;
    *found = False;
    Pointeur_As P = *R;

    while (P != NULL && !(*found)) {
        path++;
        if (strcmp(Info_As(P), *Val) == 0) {
            *found = True;
        } else if (strcmp(Info_As(P), *Val) < 0) {
            P = Fd_As(P);  // Go right if search value is greater
        } else {
            P = Fg_As(P);  // Go left if search value is smaller
        }
    }
    return path;
}

//  search function for triplet that properly counts path
int search_triplet_with_path(Pointeur_As *Bst1, Pointeur_As *Bst2, Pointeur_As *Bst3, string255 *Val, bool *found) {
    int path = 0;
    *found = False;

    // Determine which BST to search based on first character
    char first_char = (*Val)[0];

    if (first_char == 'X' || first_char == 'Y' || first_char == 'Z') {
        Search_in_bst1(Bst1, Val, &path, found);
    } else if (first_char >= 'a' && first_char <= 'w') {
        Search_in_bst2(Bst2, Val, &path, found);
    } else {
        Search_in_bst3(Bst3, Val, &path, found);
    }
    return path;
}


void print_table_header() {
    printf("\n");
    printf("=====================================================================================================\n");
    printf("                               SEARCH EFFICIENCY SIMULATION RESULTS\n");
    printf("=====================================================================================================\n");
    printf("| %-13s | %-11s | %-13s | %-15s | %-17s | %-15s | %-17s |\n",
           "Simulation nb", "Found words", "Unfound words",
           "Path length of", "Path length of", "Path length of", "Path length of");
    printf("| %-13s | %-11s | %-13s | %-15s | %-17s | %-15s | %-17s |\n",
           "", "", "", "found words in", "unfound words in", "found words in", "unfound words in");
    printf("| %-13s | %-11s | %-13s | %-15s | %-17s | %-15s | %-17s |\n",
           "", "", "", "BST0", "BST0", "triplet", "triplet");
    printf("|---------------|-------------|---------------|-----------------|-------------------|-----------------|-------------------|\n");
}

void print_table_row(int sim_num_s, SearchResults *result) {
    char sim_label_s[10];
    sprintf(sim_label_s, "    S%d", sim_num_s);

    printf("| %-13s | %-11d | %-13d | %-15d | %-17d | %-15d | %-17d |\n",
           sim_label_s,
           result->found_words_bst0,
           result->unfound_words_bst0,
           result->path_length_found_bst0,
           result->path_length_unfound_bst0,
           result->path_length_found_triplet,
           result->path_length_unfound_triplet);
    printf("|---------------|-------------|---------------|-----------------|-------------------|-----------------|-------------------|\n");
}




//  range search function to count visited nodes
void range_search_bst0_count(Pointeur_As R, string255 *word1, string255 *word2, int *visited)
{
    if (R != NULL)
    {
         (*visited)++;
        // Current node value
        string255 current_val = Info_As(R);

        // If current value is within range, it's part of the result
        if (strcmp(current_val, *word1) >= 0 && strcmp(current_val, *word2) <= 0) {
            // Node is in range - continue searching both subtrees
            Pointeur_As left = Fg_As(R);
            Pointeur_As right = Fd_As(R);

            range_search_bst0_count(left, word1, word2, visited);
            range_search_bst0_count(right, word1, word2, visited);
        }
        // If current value is less than word1, search right subtree only
        else
        if (strcmp(current_val, *word1) < 0)
        {
            Pointeur_As right = Fd_As(R);
                range_search_bst0_count(right, word1, word2, visited);
        }
        // If current value is greater than word2, search left subtree only
        else if (strcmp(current_val, *word2) > 0)
        {
            Pointeur_As left = Fg_As(R);
             range_search_bst0_count(left, word1, word2, visited);
        }
    }
    else
    {
         (*visited)++;
         return;
    }
}

//  main simulation function
void simulate_search_efficiency(int M, int N) {
    print_header("SEARCH EFFICIENCY SIMULATION");

    SearchResults *results = malloc(M * sizeof(SearchResults));

    printf("Running %d simulations with %d words each...\n", M, N);

    for (int i = 0; i < M; i++) {
        printf("Processing Simulation %d/%d...\n", i + 1, M);

        // Initialize results
        results[i].found_words_bst0 = 0;
        results[i].unfound_words_bst0 = 0;
        results[i].found_words_triplet = 0;
        results[i].unfound_words_triplet = 0;
        results[i].path_length_found_bst0 = 0;
        results[i].path_length_unfound_bst0 = 0;
        results[i].path_length_found_triplet = 0;
        results[i].path_length_unfound_triplet = 0;

        // Generate TWO DIFFERENT files
        char filename1[20], filename2[20];
        sprintf(filename1, "F_build_%d.z", i);
        sprintf(filename2, "F_search_%d.z", i);

        // Generate different files with different seeds
        generate_test_file(filename1, N);
        srand(time(NULL) + i + 1000); // Change seed for second file
        generate_test_file(filename2, N);
        srand(time(NULL)); // Reset seed

        // Build trees using FIRST file only
        Pointeur_As sim_bst0 = NULL, sim_bst1 = NULL, sim_bst2 = NULL, sim_bst3 = NULL;
        FILE *F;

        // Build BST0 from first file
        Ouvrir_s(&F, filename1, "A");
        int bst_type = 0;
        Build_bst(&sim_bst0, filename1, &bst_type);
        bst_type = 1;
        Build_bst(&sim_bst1, filename1, &bst_type);
        bst_type = 2;
        Build_bst(&sim_bst2, filename1, &bst_type);
        bst_type = 3;
        Build_bst(&sim_bst3,filename1, &bst_type);
        Fermer_s(F);

        // Search using SECOND file (different words)
        Ouvrir_s(&F, filename2, "A");
        while (!Finfich_s(F)) {
            Typestruct1_s S = malloc(255 * sizeof(char));
            Lireseq_s(F, S);

            bool found_bst0, found_triplet;
            int path_bst0 = search_bst0_with_path(&sim_bst0, &S, &found_bst0);
            int path_triplet = search_triplet_with_path(&sim_bst1, &sim_bst2, &sim_bst3, &S, &found_triplet);

            // Count BST0 results
            if (found_bst0) {
                results[i].found_words_bst0++;
                results[i].path_length_found_bst0 += path_bst0;
            } else {
                results[i].unfound_words_bst0++;
                results[i].path_length_unfound_bst0 += path_bst0;
            }

            // Count triplet results
            if (found_triplet) {
                results[i].found_words_triplet++;
                results[i].path_length_found_triplet += path_triplet;
            } else {
                results[i].unfound_words_triplet++;
                results[i].path_length_unfound_triplet += path_triplet;
            }

            free(S);
        }
        Fermer_s(F);

        Ouvrir_s(&F, filename1, "A");
        while (!Finfich_s(F)) {
            Typestruct1_s S = malloc(255 * sizeof(char));
            Lireseq_s(F, S);

            bool found_bst0, found_triplet;
            int path_bst0 = search_bst0_with_path(&sim_bst0, &S, &found_bst0);
            int path_triplet = search_triplet_with_path(&sim_bst1, &sim_bst2, &sim_bst3, &S, &found_triplet);

            // Count BST0 results
            if (found_bst0) {
                results[i].found_words_bst0++;
                results[i].path_length_found_bst0 += path_bst0;
            } else {
                results[i].unfound_words_bst0++;
                results[i].path_length_unfound_bst0 += path_bst0;
            }

            // Count triplet results
            if (found_triplet) {
                results[i].found_words_triplet++;
                results[i].path_length_found_triplet += path_triplet;
            } else {
                results[i].unfound_words_triplet++;
                results[i].path_length_unfound_triplet += path_triplet;
            }

            free(S);
        }
        Fermer_s(F);

        // Clean up trees
        Free_tree(&sim_bst0);
        Free_tree(&sim_bst1);
        Free_tree(&sim_bst2);
        Free_tree(&sim_bst3);
    }

    // Display results in  table
    print_table_header();

    for (int i = 0; i < M; i++) {
        print_table_row(i + 1, &results[i]);
    }

    printf("\nLegend:\n");
    printf("Found words    = Words that exist in the tree\n");
    printf("Unfound words  = Words that do not exist in the tree\n");
    printf("Path length    = Total length of search paths traversed\n");

    free(results);

}

// Structure for range search results
typedef struct {
    int total_nodes_visited_bst0;
    int total_nodes_visited_triplet;
    int number_of_ranges;
} RangeSearchResults;

//  range search for triplet
int range_search_triplet_count(Pointeur_As *Bst1, Pointeur_As *Bst2, Pointeur_As *Bst3, string255 *word1, string255 *word2)
{
    int visited = 0;

    // Determine which BST to use based on first character of word1
    char first_char = (*word1)[0];

    if (first_char == 'x' || first_char == 'Y' || first_char == 'Z') {
        range_search_bst0_count(*Bst1, word1, word2, &visited);
    } else if (first_char > 'x' || first_char > 'Y' || first_char >'Z') {
        range_search_bst0_count(*Bst2, word1, word2, &visited);
    } else {
        range_search_bst0_count(*Bst3, word1, word2, &visited);
    }

    return visited;
}

void print_table_header_range() {
    printf("\n");
    printf("================================================================================\n");
    printf("                        RANGE SEARCH SIMULATION RESULTS\n");
    printf("================================================================================\n");
    printf("| %-12s | %-35s | %-35s |\n", "Simulation", "BST0", "(BST1, BST2, BST3)");
    printf("|--------------|-------------------------------------|-------------------------------------|\n");
    printf("| %-12s | %-35s | %-35s |\n", "", "Total Number of Nodes", "Total Number of Nodes");
    printf("| %-12s | %-35s | %-35s |\n", "", "Visited", "Visited");
    printf("|--------------|-------------------------------------|-------------------------------------|\n");
}

void print_table_row_range(int sim_num, RangeSearchResults *result) {
    char sim_label[10];
    sprintf(sim_label, "S%d", sim_num);

    printf("| %-12s | %-35d | %-35d |\n",
           sim_label,
           result->total_nodes_visited_bst0,
           result->total_nodes_visited_triplet);
    printf("|--------------|-------------------------------------|-------------------------------------|\n");
}

void simulate_range_search_efficiency(int M, int N) {
    print_header("RANGE SEARCH EFFICIENCY SIMULATION");

    RangeSearchResults *results = malloc(M * sizeof(RangeSearchResults));

    printf("Running %d simulations with %d words each (%d range pairs)...\n", M, N, N/2);

    for (int i = 0; i < M; i++) {
        printf("Processing Simulation %d/%d...\n", i + 1, M);

        // Initialize results
        results[i].total_nodes_visited_bst0 = 0;
        results[i].total_nodes_visited_triplet = 0;
        results[i].number_of_ranges = N/2;

        // Generate files
        char filename1[20], filename2[20];
        sprintf(filename1, "F_range_%d.z", i);
        sprintf(filename2, "F2_range_%d.z", i);

        generate_test_file(filename1, N);
        generate_word_pairs_file(filename2, N/2);

        // Build trees
        Pointeur_As sim_bst0 = NULL, sim_bst1 = NULL, sim_bst2 = NULL, sim_bst3 = NULL;
        FILE *F;

        int bst_type=0;
        Build_bst(&sim_bst0, filename1, &bst_type);
        bst_type = 1;
        Build_bst(&sim_bst1, filename1, &bst_type);
        bst_type = 2;
        Build_bst(&sim_bst2, filename1, &bst_type);
        bst_type = 3;
        Build_bst(&sim_bst3, filename1, &bst_type);

        // Perform range searches
        Ouvrir_s(&F, filename2, "A");
        while (!Finfich_s(F)) {
            Typestruct1_s S = malloc(255 * sizeof(char));
            Lireseq_s(F, S);

            // Parse word1 and word2 from "word1,word2"
            char *word1 = strtok(S, ",");
            char *word2 = strtok(NULL, ",");

            if (word1 && word2) {
                // Create copies since strtok modifies the original string
                string255 w1 = malloc(255 * sizeof(char));
                string255 w2 = malloc(255 * sizeof(char));
                strcpy(w1, word1);
                strcpy(w2, word2);

                // Ensure proper ordering
                if (strcmp(w1, w2) > 0) {
                    string255 temp = malloc(255 * sizeof(char));
                    strcpy(temp, w1);
                    strcpy(w1, w2);
                    strcpy(w2, temp);
                    free(temp);
                }

                int visited_bst0 = 0;
                range_search_bst0_count(sim_bst0, &w1, &w2, &visited_bst0);
                results[i].total_nodes_visited_bst0 += visited_bst0;

                int visited_triplet = range_search_triplet_count(&sim_bst1, &sim_bst2, &sim_bst3, &w1, &w2);
                results[i].total_nodes_visited_triplet += visited_triplet;

                free(w1);
                free(w2);
            }
            free(S);
        }
        Fermer_s(F);

        // Clean up trees
        Free_tree(&sim_bst0);
        Free_tree(&sim_bst1);
        Free_tree(&sim_bst2);
        Free_tree(&sim_bst3);
    }

    // Display results in table format
    print_table_header_range();

    for (int i = 0; i < M; i++) {
        print_table_row_range(i + 1, &results[i]);
    }

    printf("\nNote: Values represent total number of nodes visited during range search operations.\n");


    free(results);
}


  int main(int argc, char *argv[])
{
    srand(time(NULL));
    S = malloc(255 * sizeof(char));
    Val = malloc(255 * sizeof(char));

    print_header("WELCOME TO ENHANCED TP Z\n                              By Benrabah Hamza and Bnekerri Yacine\n                              Section : D    Group : 15  ");

    Bst0 = NULL;
    Bst1 = NULL;
    Bst2 = NULL;
    Bst3 = NULL;
    Choix = 1;

    while ((Choix > 0) && (Choix < 14)) {
        printf("\n");
        printf("******************************MENU*****************************\n");
        printf("*1-GENERATE RANDOM FILE OF WORDS                              *\n");
        printf("*2-BUILD BST0                                                 *\n");
        printf("*3-BUILD BST1,BST2,BST3                                       *\n");
        printf("*4-Depth of the trees                                         *\n");
        printf("*5-Word search                                                *\n");
        printf("*6-Count of words starting with x,Y,Z                         *\n");
        printf("*7-Range search                                               *\n");
        printf("*8-Count by level BST1                                        *\n");
        printf("*9-Count by level BST2                                        *\n");
        printf("*10-Count by level BST3                                       *\n");
        printf("*11-SEARCH EFFICIENCY SIMULATION                              *\n");
        printf("*12-RANGE SEARCH EFFICIENCY SIMULATION                        *\n");
        printf("*13-TREE VISUALIZATION                                        *\n");
        printf("*14-CLOSE                                                     *\n");
        printf("***************************************************************\n");

        printf("Enter your choice: ");
        scanf(" %d", &Choix);
       if( ( Choix == 1 )) {
         clearScreen();
         Generateandreadfile ( & F ) ;


       } ;
       if( ( Choix == 2 )) {
            clearScreen();
         For_count  =  False ;
         Free_tree ( & Bst0 ) ;
         Bst  =  0 ;
         Build_bst ( & Bst0 ,  "F2.z" , & Bst ) ;
         Inorder ( & Bst0 , & For_count , & X , & Y , & Z ) ;


       } ;
       if( ( Choix == 3 )) {
            clearScreen();
         Free_tree ( & Bst1 ) ;

         Bst  =  1 ;
         Build_bst ( & Bst1 , "F2.z" , & Bst ) ;


         For_count  =  False ;
         Inorder ( & Bst1 , & For_count , & X , & Y , & Z ) ;

         printf ( " %s", "-----------------------------------------------------------\n" ) ;
         printf ( " %s", "-----------------------------------------------------------\n" ) ;
         Free_tree ( & Bst2 ) ;
         Bst  =  2 ;
         Build_bst ( & Bst2 , "F2.z" , & Bst ) ;
         Inorder ( & Bst2 , & For_count , & X , & Y , & Z ) ;

         printf ( " %s", "-----------------------------------------------------------\n" ) ;
         printf ( " %s", "-----------------------------------------------------------\n" ) ;
         Free_tree ( & Bst3 ) ;
         Bst  =  3 ;
         Build_bst ( & Bst3 ,"F2.z", & Bst ) ;

         Inorder ( & Bst3 , & For_count , & X , & Y , & Z ) ;


       } ;
       if( ( Choix == 4 )) {
            clearScreen();
         printf ( " %s", "\nDepth of BST0 is:" ) ;
         printf ( " %d", Depth(&Bst0) ) ;
         printf ( " %s", "\nDepth of BST1 is:" ) ;
         printf ( " %d", Depth(&Bst1) ) ;
         printf ( " %s", "\nDepth of BST2 is:" ) ;
         printf ( " %d", Depth(&Bst2) ) ;
         printf ( " %s", "\nDepth of BST3 is:" ) ;
         printf ( " %d\n", Depth(&Bst3) ) ;

       } ;
       if( ( Choix == 5 )) {
           clearScreen();
         printf ( " %s", "Enter the word to search for : " ) ;
         scanf ( " %[^\n]", Val ) ;
         Word_search ( & Bst1 , & Bst2 , & Bst3 , & Val ) ;

       } ;
       if( ( Choix == 6 )) {
            clearScreen();
         printf ( " %s", "\nFor BST1:\n" ) ;
         Count ( & Bst1 ) ;
         printf ( " %s", "\nFor BST2:\n" ) ;
         Count ( & Bst2 ) ;
         printf ( " %s", "\nFor BST3:\n" ) ;
         Count ( & Bst3 ) ;
         printf("\n");


       } ;
       if( ( Choix == 7 )) {
            clearScreen();
         Rang_search ( & Bst1 , & Bst2 , & Bst3 ) ;
            printf("\n");

       } ;
       if( ( Choix == 8 )) {
            clearScreen();
         Count_by_level ( & Bst1 ) ;
            printf("\n");

       } ;
       if( ( Choix == 9 )) {
            clearScreen();
         Count_by_level ( & Bst2 ) ;
            printf("\n");

       } ;
       if( ( Choix == 10 )) {
            clearScreen();
         Count_by_level ( & Bst3 ) ;
            printf("\n");

       } ;
if (Choix == 11) {
        clearScreen();
            int M, N;
            printf("Enter number of simulations (M >= 10): ");
            scanf("%d", &M);
            printf("Enter number of words per simulation (N >= 10000): ");
            scanf("%d", &N);

            if (M >= 10 && N >= 10000) {
                simulate_search_efficiency(M, N);
            } else {
                printf("Invalid parameters. M must be >= 10 and N must be >= 10000.\n");

            }
               printf("\n");

        }

        if (Choix == 12) {
            int M, N;
 clearScreen();
            printf("Enter number of simulations (M >= 10): ");
            scanf("%d", &M);
            printf("Enter number of words per simulation (N >= 10000): ");
            scanf("%d", &N);

            if (M >= 10 && N >= 10000) {
                simulate_range_search_efficiency(M, N);
            } else {
                printf("Invalid parameters. M must be >= 10 and N must be >= 10000.\n");
            }
               printf("\n");
        }

        if (Choix == 13) {
                clearScreen();
            print_header("TREE VISUALIZATION");
            if (Bst0) {
                printf("\nBST0 Structure:\n");
                inorder_display(&Bst0, 0);
                print_tree_info("BST0", Bst0);
            }
            if (Bst1) {
                printf("\nBST1 Structure:\n");
                inorder_display(&Bst1, 0);
                print_tree_info("BST1", Bst1);
            }
            if (Bst2) {
                printf("\nBST2 Structure:\n");
               inorder_display(&Bst2, 0);
                print_tree_info("BST2", Bst2);
            }
            if (Bst3) {
                printf("\nBST3 Structure:\n");
                inorder_display(&Bst3, 0);
                print_tree_info("BST3", Bst3);
            }
               printf("\n");
        }
    }

    Free_tree(&Bst0);
    Free_tree(&Bst1);
    Free_tree(&Bst2);
    Free_tree(&Bst3);

    system("PAUSE");
    return 0;
}
