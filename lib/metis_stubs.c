#include<caml/mlvalues.h>
#include<caml/memory.h>
#include<caml/alloc.h>
#include<caml/custom.h>
#include<caml/fail.h>
#include<metis.h>
#include<stdlib.h>
#include<stdio.h>

static unsigned char set_options = 0; 

value 
Metis_PartGraphKWayNative (value v_nvtxs, value v_cons, value v_xadj, value v_adjncy,
			   value v_vwgts, value v_vsize, value v_adjwts, value v_nparts,
			   value v_tpwgts, value v_ubvec);
value 
Metis_PartGraphRecursiveNative (value v_nvtxs, value v_cons, value v_xadj, value v_adjncy,
				value v_vwgts, value v_vsize, value v_adjwts, value v_nparts,
				value v_tpwgts, value v_ubvec);

value
Metis_PartMeshDualNative (value v_ne, value v_nn, value v_eptr, value v_eind,
			  value v_vwgt, value v_vsize, value v_ncommon, value v_nparts,
			  value v_tpwgts);

value
Metis_PartMeshNodalNative (value v_ne, value v_nn, value v_eptr, value v_eind,
			   value v_vwgt, value v_vsize, value v_nparts,
			   value v_tpwgts);

void
get_real_array_values (value val, real_t *array) {
  CAMLparam1(val);
  /* Now get the arrays in*/
  if (Tag_val (val) == Double_array_tag){
    int i =0;
    for (i=0; i<Wosize_val(val);++i){
      /* Get the values */
      array[i] = Double_field(val,i);
    }
  }
  CAMLreturn0;
}

void
get_array_values (value val, idx_t *array) {
  CAMLparam1(val);
  /* Now get the arrays in*/
  if (Tag_val (val) == 0){
    int i =0;
    for (i=0; i<Wosize_val(val);++i){
      /* Get the values */
      array[i] = Nativeint_val(Field(val,i));
    }
  }
  CAMLreturn0;
}

void
Metis_SetDefaultOptions (value options){
  CAMLparam1 (options);
  if(Tag_val (options) == 0) {
    if (Wosize_val (options) < METIS_NOPTIONS) caml_failwith ("argument length less than METIS_NOPTIONS");
    else{
      idx_t moptions[METIS_NOPTIONS];
      int i = 0;
      for(i=0;i<METIS_NOPTIONS;++i)
	moptions[i] = (idx_t)(Nativeint_val(Field(options,i)));
      set_options = 1;
      METIS_SetDefaultOptions (moptions);
    }
  } else caml_failwith ("argument not of type nativeint array");
  CAMLreturn0;
}
/*
  The arguments compared to Native part
  argv[0] = v_nvtxs
  argv[1] = v_cons
  argv[2] = v_xadj
  argv[3] = v_adjncy
  argv[4] = v_vwgts
  argv[5] = v_vsize
  argv[6] = v_adjwts
  argv[7] = v_nparts
  argv[8] = v_tpwgts
  argv[9] = v_ubvec

*/
value 
Metis_PartGraphRecursiveBytecode (value *argv, int argn) {

  CAMLparamN (argv,argn);
  
  Metis_PartGraphRecursiveNative (argv[0],argv[1], argv[2], argv[3], argv[4],
				  argv[5], argv[6], argv[7], argv[8], argv[9]);

  /* First get the int values */
  /* idx_t nvtxs = (idx_t)((Nativeint_val(argv[0]))); */
  /* idx_t ncon = (idx_t)((Nativeint_val(argv[1]))); */
  /* idx_t nparts = (idx_t)((Nativeint_val(argv[7]))); */
  
  /* /\* Get all the arrays *\/ */
  /* if(Tag_val(argv[2]) == 0 && Tag_val(argv[3])==0 && Tag_val(argv[4]) ==0 && Tag_val(argv[6])==0){ */
  /*   /\* Use mallocs, because things might overflow!! *\/ */

  /*   /\* Getting xadj *\/ */
  /*   idx_t *xadj = (idx_t*)(malloc((Wosize_val(argv[2])*sizeof(idx_t)))); */
  /*   get_array_values(argv[2],xadj); */

  /*   /\* Getting adjncy *\/ */
  /*   idx_t *adjncy = (idx_t*)(malloc((Wosize_val(argv[3])*sizeof(idx_t)))); */
  /*   get_array_values(argv[3],adjncy); */

  /*   /\* Getting vsize *\/ */
  /*   idx_t *vsize = (idx_t*)(malloc((Wosize_val(argv[5])*sizeof(idx_t)))); */
  /*   get_array_values(argv[5],vsize); */
    
  /*   /\* Getting vwgts *\/ */
  /*   idx_t *vwgts = (idx_t*)(malloc((Wosize_val(argv[4])*sizeof(idx_t)))); */
  /*   get_array_values(argv[4],vwgts); */
    
  /*   /\* Getting adjwgt *\/ */
  /*   idx_t *adjwt = (idx_t*)(malloc((Wosize_val(argv[6])*sizeof(idx_t)))); */
  /*   get_array_values(argv[6],adjwt); */
    
  /*   /\* Getting tpwgt *\/ */
  /*   real_t *tpwgts = (real_t*)(malloc((Wosize_val(argv[8])*sizeof(real_t)))); */
  /*   //Check the size */
  /*   if (Wosize_val(argv[8]) != (nparts*ncon)) caml_failwith ("Argument 5 size not equal to (arg2 * arg8)"); */
  /*   get_real_array_values(argv[8],tpwgts); */
    
  /*   /\* Getting ubvec *\/ */
  /*   real_t *ubvec = (real_t*)(malloc((Wosize_val(argv[9])*sizeof(real_t)))); */
  /*   //Check the size  */
  /*   if (Wosize_val(argv[9]) != ncon) caml_failwith ("Argument 5 size not equal to arg2"); */
  /*   get_real_array_values(argv[9],ubvec); */
    
  /*   /\* Now make the return array *\/ */
  /*   idx_t objval; */
    
  /*   /\* Now make the return array *\/ */
  /*   idx_t *part = (idx_t*)malloc(sizeof(idx_t)*(nvtxs)); */
    
  /*   idx_t options [METIS_NOPTIONS]; */
  /*   if (!set_options) */
  /*     METIS_SetDefaultOptions (options); */
  /*   options[METIS_OPTION_DBGLVL] = 1; */
  /*   /\* Now make a call the metis library function *\/ */
  /*   int ret_val = METIS_PartGraphRecursive(&nvtxs,&ncon,xadj,adjncy,vwgts,vsize,adjwt,&nparts, */
  /* 					   tpwgts,ubvec,options,&objval,part); */
  /*   if (ret_val == METIS_OK){ */
  /*     /\*First free all the allocations that we have done, so that there is no memory leak!! *\/ */
  /*     free (xadj); */
  /*     free (adjwt); */
  /*     free (adjncy); */
  /*     free (vwgts); */
  /*     free (tpwgts); */
  /*     free (ubvec); */

  /*     /\* Now we want to make the tuple that returns the value back in to Ocaml's heap space *\/ */
  /*     /\* DRAGON: This tuple might be garbage collected by the Ocaml GC!! *\/ */
  /*     CAMLlocal3(ret_objval,ret_part,ret_tuple); */
  /*     ret_objval = caml_copy_nativeint (objval); //The objective value to be sent back */
  /*     ret_part = caml_alloc (nvtxs,0); //The npart array (snd ret_tuple) */
  /*     ret_tuple = caml_alloc_tuple (2); //The tuple to send back */
      
  /*     /\* Fill in the return array *\/ */
  /*     int i=0; */
  /*     for(i=0;i<nvtxs;++i) */
  /* 	Store_field (ret_part,i,caml_copy_nativeint(part[i])); */
      
  /*     free(part); */
  /*     /\* Fill in the tuple for return *\/ */
      
  /*     Store_field (ret_tuple,0,ret_objval); */
  /*     Store_field (ret_tuple,1,ret_part); */
      
  /*     CAMLreturn(ret_tuple); */
  /*   } */
  /*   else if (ret_val == METIS_ERROR_INPUT) caml_failwith ("METIS FAILED WITH METIS_ERROR_INPUT"); */
  /*   else if (ret_val == METIS_ERROR_MEMORY) caml_failwith ("METIS FAILED WITH METIS_ERROR_MEMORY"); */
  /*   else if (ret_val == METIS_ERROR) caml_failwith ("METIS FAILED WITH METIS_ERROR"); */
  /* } */
  /* else caml_failwith ("One of the array arguments is incorrect"); */
}

value 
Metis_PartGraphKWayBytecode (value *argv, int argn){

  CAMLparamN (argv,argn);

  /*Just call the native function with all the arguments out */
  Metis_PartGraphKWayNative (argv[0], argv[1], argv[2], argv[3], argv[4],
			     argv[5], argv[6],argv[7],argv[8],
			     argv[9]);

  /* First get the int values */
/*   idx_t nvtxs = (idx_t)((Nativeint_val(argv[0]))); */
/*   idx_t ncon = (idx_t)((Nativeint_val(argv[1]))); */
/*   idx_t nparts = (idx_t)((Nativeint_val(argv[7]))); */
  
/* #ifdef DEBUG */
/*   fprintf(stdout,"nvtxs: %d, ncon: %d, nparts: %d\n",nvtxs,ncon,nparts); */
/* #endif */
/*   /\* Get all the arrays *\/ */
/*   if(Tag_val(argv[2]) == 0 && Tag_val(argv[3])==0 && Tag_val(argv[4]) ==0 && Tag_val(argv[6])==0){ */
/*     /\* Use mallocs, because things might overflow!! *\/ */

/* #ifdef DEBUG */
/*     fprintf(stdout,"All blocks are OK with their tags!!"); */
/* #endif */

/*     /\* Getting xadj *\/ */
/*     idx_t *xadj = (idx_t*)(calloc(Wosize_val(argv[2]),sizeof(idx_t))); */
/*     get_array_values(argv[2],xadj); */

/* #ifdef DEBUG */
/*     int tutu=0; */
/*     fprintf(stdout, "XADJ FROM C\n"); */
/*     for (tutu=0;tutu<Wosize_val(argv[2]);++tutu) */
/*       fprintf(stdout, "%d ", xadj[tutu]); */
/*     fprintf(stdout,"\n"); */
/* #endif */

/*     /\* Getting adjncy *\/ */
/*     idx_t *adjncy = (idx_t*)(malloc((Wosize_val(argv[3])*sizeof(idx_t)))); */
/*     get_array_values(argv[3],adjncy); */

/*     idx_t *vsize = (idx_t*)(malloc((Wosize_val(argv[5])*sizeof(idx_t)))); */
/*     if (Wosize_val (argv[5]) == 0) */
/*       vsize = NULL; */
/*     get_array_values(argv[5],vsize); */
    
/*     /\* Getting vwgts *\/ */
/*     idx_t *vwgts = (idx_t*)(malloc((Wosize_val(argv[4])*sizeof(idx_t)))); */
/*     if (Wosize_val (argv[4]) == 0) */
/*       vwgts = NULL; */
/*     else get_array_values(argv[4],vwgts); */
    
/*     /\* Getting adjwgt *\/ */
/*     idx_t *adjwt = (idx_t*)(malloc((Wosize_val(argv[6])*sizeof(idx_t)))); */
/*     get_array_values(argv[6],adjwt); */
    
/*     /\* Getting tpwgt *\/ */
/*     real_t *tpwgts = (real_t*)(malloc((Wosize_val(argv[8])*sizeof(real_t)))); */
/*     //Check the size  */
/*     if (Wosize_val(argv[8]) ==0) */
/*       tpwgts = NULL; */
/*     else if (Wosize_val(argv[8]) != (nparts*ncon)) caml_failwith ("Argument 5 size not equal to (arg2 * arg8)"); */
/*     else get_real_array_values(argv[8],tpwgts); */
    
/*     /\* Getting ubvec *\/ */
/*     real_t *ubvec = (real_t*)(malloc((Wosize_val(argv[9])*sizeof(real_t)))); */
/*     //Check the size  */
/*     if (Wosize_val(argv[9]) ==0) */
/*       ubvec = NULL; */
/*     else if (Wosize_val(argv[9]) != ncon) caml_failwith ("Argument 5 size not equal to arg2"); */
/*     else get_real_array_values(argv[9],ubvec); */
    
/*     /\* Now make the return array *\/ */
/*     idx_t objval; */
    
/*     /\* Now make the return array *\/ */
/*     idx_t *part = (idx_t*)malloc(sizeof(idx_t)*(nvtxs)); */
    
/*     /\* Now make a call the metis library function *\/ */
/*     idx_t options [METIS_NOPTIONS]; */
/*     if (!set_options) */
/*       METIS_SetDefaultOptions (options); */
/*     options[METIS_OPTION_DBGLVL] = 1; */
/*     int ret_val = METIS_PartGraphKway(&nvtxs,&ncon,xadj,adjncy,vwgts,vsize,adjwt,&nparts, */
/* 					   tpwgts,ubvec,options,&objval,part); */
/*     if (ret_val == METIS_OK){ */
/*       /\*First free all the allocations that we have done, so that there is no memory leak!! *\/ */
/*       free (xadj); */
/*       free (adjwt); */
/*       free (adjncy); */
/*       free (vwgts); */
/*       free (tpwgts); */
/*       free (ubvec); */

/*       /\* Now we want to make the tuple that returns the value back in to Ocaml's heap space *\/ */
/*       /\* DRAGON: This tuple might be garbage collected by the Ocaml GC!! *\/ */
/*       CAMLlocal3(ret_objval,ret_part,ret_tuple); */
/*       ret_objval = caml_copy_nativeint (objval); //The objective value to be sent back */
/*       ret_part = caml_alloc (nvtxs,0); //The npart array (snd ret_tuple) */
/*       ret_tuple = caml_alloc_tuple (2); //The tuple to send back */
      
/*       /\* Fill in the return array *\/ */
/*       int i=0; */
/*       for(i=0;i<nvtxs;++i) */
/* 	Store_field (ret_part,i,caml_copy_nativeint(part[i])); */

/*       free(part); */
      
/*       /\* Fill in the tuple for return *\/ */
      
/*       Store_field (ret_tuple,0,ret_objval); */
/*       Store_field (ret_tuple,1,ret_part); */
      
/*       CAMLreturn(ret_tuple); */
/*     } */
/*     else if (ret_val == METIS_ERROR_INPUT) caml_failwith ("METIS FAILED WITH METIS_ERROR_INPUT"); */
/*     else if (ret_val == METIS_ERROR_MEMORY) caml_failwith ("METIS FAILED WITH METIS_ERROR_MEMORY"); */
/*     else if (ret_val == METIS_ERROR) caml_failwith ("METIS FAILED WITH METIS_ERROR"); */
/*   } */
/*   else caml_failwith ("One of the array arguments is incorrect"); */
}

value 
Metis_PartGraphRecursiveNative (value v_nvtxs, value v_cons, value v_xadj, value v_adjncy,
				value v_vwgts, value v_vsize, value v_adjwts, value v_nparts,
				value v_tpwgts, value v_ubvec) {

  CAMLparam5(v_nvtxs,v_cons,v_xadj,v_adjncy,v_vwgts);
  CAMLxparam5(v_vsize,v_adjwts,v_nparts,v_tpwgts,v_ubvec);
  /* First get the int values */
  idx_t nvtxs = (idx_t)((Nativeint_val(v_nvtxs)));
  idx_t ncon = (idx_t)((Nativeint_val(v_cons)));
  idx_t nparts = (idx_t)((Nativeint_val(v_nparts)));
  
  /* Get all the arrays */
  if(Tag_val(v_xadj) == 0 && Tag_val(v_adjncy)==0 && Tag_val(v_vwgts) ==0 && Tag_val(v_adjwts)==0){
    /* Use mallocs, because things might overflow!! */

    /* Getting xadj */
    idx_t *xadj = (idx_t*)(malloc((Wosize_val(v_xadj)*sizeof(idx_t))));
    get_array_values(v_xadj,xadj);

    /* Getting adjncy */
    idx_t *adjncy = (idx_t*)(malloc((Wosize_val(v_adjncy)*sizeof(idx_t))));
    get_array_values(v_adjncy,adjncy);

    /* Getting vwgts */
    idx_t *vsize = (idx_t*)(malloc((Wosize_val(v_vsize)*sizeof(idx_t))));
    get_array_values(v_vsize,vsize);

    /* Getting vwgts */
    idx_t *vwgts = (idx_t*)(malloc((Wosize_val(v_vwgts)*sizeof(idx_t))));
    get_array_values(v_vwgts,vwgts);
    
    /* Getting adjwgt */
    idx_t *adjwt = (idx_t*)(malloc((Wosize_val(v_adjwts)*sizeof(idx_t))));
    get_array_values(v_adjwts,adjwt);
    
    /* Getting tpwgt */
    real_t* tpwgts = (real_t*)(malloc((Wosize_val(v_tpwgts)*sizeof(real_t))));
    //Check the size 
    if (Wosize_val(v_tpwgts) != (nparts*ncon)) caml_failwith ("Argument 5 size not equal to (arg2 * arg8)");
    get_real_array_values(v_tpwgts,tpwgts);
    
    /* Getting ubvec */
    real_t *ubvec = (real_t*)(malloc((Wosize_val(v_ubvec)*sizeof(real_t))));
    //Check the size 
    if (Wosize_val(v_ubvec) !=0 && Wosize_val(v_ubvec) != ncon) caml_failwith ("Argument 5 size not equal to arg2");
    get_real_array_values(v_ubvec,ubvec);
    
    /* Now make the return array */
    idx_t objval;
    
    /* Now make the return array */
    idx_t *part = (idx_t*)malloc(sizeof(idx_t)*(nvtxs));
    
    /* Now make a call the metis library function */
    int ret_val = METIS_PartGraphRecursive(&nvtxs,&ncon,xadj,adjncy,vwgts,vsize,adjwt,&nparts,
					   tpwgts,ubvec,NULL,&objval,part);
    if (ret_val == METIS_OK){
      /*First free all the allocations that we have done, so that there is no memory leak!! */
      free (xadj);
      free (adjwt);
      free (adjncy);
      free (vwgts);
      free (tpwgts);
      free (ubvec);

      /* Now we want to make the tuple that returns the value back in to Ocaml's heap space */
      /* DRAGON: This tuple will be garbage collected by the Ocaml GC!! */
      CAMLlocal3(ret_objval,ret_part,ret_tuple);
      ret_objval = caml_copy_nativeint (objval); //The objective value to be sent back
      ret_part = caml_alloc (nvtxs,0); //The npart array (snd ret_tuple)
      ret_tuple = caml_alloc_tuple (2); //The tuple to send back
      
      /* Fill in the return array */
      int i=0;
      for(i=0;i<nvtxs;++i)
	Store_field (ret_part,i,caml_copy_nativeint(part[i]));
      
      free(part);
      /* Fill in the tuple for return */
      
      Store_field (ret_tuple,0,ret_objval);
      Store_field (ret_tuple,1,ret_part);
      
      CAMLreturn(ret_tuple);
    }
    else if (ret_val == METIS_ERROR_INPUT) caml_failwith ("METIS FAILED WITH METIS_ERROR_INPUT");
    else if (ret_val == METIS_ERROR_MEMORY) caml_failwith ("METIS FAILED WITH METIS_ERROR_MEMORY");
    else if (ret_val == METIS_ERROR) caml_failwith ("METIS FAILED WITH METIS_ERROR");
  }
  else caml_failwith ("One of the array arguments is incorrect");
}

value 
Metis_PartGraphKWayNative (value v_nvtxs, value v_cons, value v_xadj, value v_adjncy,
				value v_vwgts, value v_vsize, value v_adjwts, value v_nparts,
				value v_tpwgts, value v_ubvec) {

  CAMLparam5(v_nvtxs,v_cons,v_xadj,v_adjncy,v_vwgts);
  CAMLxparam5(v_vsize,v_adjwts,v_nparts,v_tpwgts,v_ubvec);
  /* First get the int values */
  idx_t nvtxs = (idx_t)((Nativeint_val(v_nvtxs)));
  idx_t ncon = (idx_t)((Nativeint_val(v_cons)));
  idx_t nparts = (idx_t)((Nativeint_val(v_nparts)));
  
#ifdef DEBUG
  fprintf(stdout,"nvtxs: %d, ncon: %d, nparts: %d\n",nvtxs,ncon,nparts);
#endif

  /* Get all the arrays */
  if(Tag_val(v_xadj) == 0 && Tag_val(v_adjncy)==0 && Tag_val(v_vwgts) ==0 && Tag_val(v_adjwts)==0){
    /* Use mallocs, because things might overflow!! */

#ifdef DEBUG
    fprintf(stdout,"All blocks are OK with their tags!!");
#endif

    /* Getting xadj */
    idx_t *xadj = (idx_t*) calloc(Wosize_val(v_xadj),sizeof(idx_t));
    get_array_values(v_xadj,xadj);

#ifdef DEBUG
    int tutu=0;
    fprintf(stdout, "XADJ FROM C\n");
    for (tutu=0;tutu<Wosize_val(v_xadj);++tutu)
      fprintf(stdout, "%d ", xadj[tutu]);
    fprintf(stdout,"\n");
#endif

    /* Getting adjncy */
    idx_t *adjncy = (idx_t*)(calloc(Wosize_val(v_adjncy),sizeof(idx_t)));
    get_array_values(v_adjncy,adjncy);

    /* Getting adjwgt */
    idx_t *adjwt = (idx_t*)(calloc(Wosize_val(v_adjwts),sizeof(idx_t)));
    get_array_values(v_adjwts,adjwt);

#ifdef DEBUG
    tutu=0;
    fprintf(stdout, "ADJNCY/ADJWT FROM C\n");
    for (tutu=0;tutu<Wosize_val(v_adjncy);++tutu)
      fprintf(stdout, "%d %d ", adjncy[tutu], adjwt[tutu]);
    fprintf(stdout,"\n");
#endif

    /* Getting vwgts */
    idx_t *vsize = (idx_t*)(calloc(Wosize_val(v_vsize),sizeof(idx_t)));
    if (Wosize_val (v_vsize) == 0)
      vsize = NULL;
    get_array_values(v_vsize,vsize);

    /* Getting vwgts */
    idx_t *vwgts = (idx_t*)(calloc(Wosize_val(v_vwgts),sizeof(idx_t)));
    if (Wosize_val (v_vwgts) == 0)
      vwgts = NULL;
    else get_array_values(v_vwgts,vwgts);
    
#ifdef DEBUG
    tutu=0;
    fprintf(stdout, "VWGTS FROM C\n");
    for (tutu=0;tutu<Wosize_val(v_vwgts);++tutu)
      fprintf(stdout, "%d ", vwgts[tutu]);
    fprintf(stdout,"\n");
#endif

    
    /* Getting tpwgt */
    real_t* tpwgts = (real_t*)(calloc(Wosize_val(v_tpwgts),sizeof(real_t)));
    //Check the size 
    if (Wosize_val(v_tpwgts) ==0)
      tpwgts = NULL;
    else if (Wosize_val(v_tpwgts) != (nparts*ncon)) caml_failwith ("Argument 5 size not equal to (arg2 * arg8)");
    else get_real_array_values(v_tpwgts,tpwgts);
    
#ifdef DEBUG
    /* print all the values that have been allocated!! */
#endif
    
    /* Getting ubvec */
    real_t *ubvec = (real_t*)(calloc(Wosize_val(v_ubvec),sizeof(real_t)));
    //Check the size 
    if (Wosize_val(v_ubvec) ==0)
      ubvec = NULL;
    else if (Wosize_val (v_ubvec) != 0 && Wosize_val(v_ubvec) != ncon) caml_failwith ("Argument 5 size not equal to arg2");
    else get_real_array_values(v_ubvec,ubvec);
    
    /* Now make the return array */
    idx_t objval;
    
    /* Now make the return array */
    idx_t *part = (idx_t*)malloc(sizeof(idx_t)*(nvtxs));
    
    /* Now make a call the metis library function */
    /* First set all the ones with */
#ifdef DEBUG
    fprintf(stdout,"Calling metis\n");
#endif
    idx_t options [METIS_NOPTIONS];
    if (!set_options) METIS_SetDefaultOptions (options);
    /* options[METIS_OPTION_UFACTOR] = 1.030; */
    options[METIS_OPTION_DBGLVL] = 1;
    /* options[METIS_OPTION_NCUTS] = 4; */
    /* options[METIS_OPTION_CTYPE] = METIS_CTYPE_SHEM; */
    /* options[METIS_OPTION_IPTYPE] = METIS_IPTYPE_NODE; */
    int ret_val = METIS_PartGraphKway(&nvtxs,&ncon,xadj,adjncy,vwgts,vsize,adjwt,&nparts,
					   tpwgts,ubvec,options,&objval,part);
    if (ret_val == METIS_OK){
#ifdef DEBUG
      fprintf(stdout, "METIS RAN OK\n");
#endif
      /*First free all the allocations that we have done, so that there is no memory leak!! */
      free (xadj);
      free (adjwt);
      free (adjncy);
      free (vwgts);
      free (tpwgts);
      free (ubvec);

      /* Now we want to make the tuple that returns the value back in to Ocaml's heap space */
      /* DRAGON: This tuple will be garbage collected by the Ocaml GC!! */
      CAMLlocal3(ret_objval,ret_part,ret_tuple);
      ret_objval = caml_copy_nativeint (objval); //The objective value to be sent back
      ret_part = caml_alloc (nvtxs,0); //The npart array (snd ret_tuple)
      ret_tuple = caml_alloc_tuple (2); //The tuple to send back
      
      /* Fill in the return array */
      int i=0;
      for(i=0;i<nvtxs;++i)
	Store_field (ret_part,i,caml_copy_nativeint(part[i]));
      
      free(part);
      
      /* Fill in the tuple for return */
      
      Store_field (ret_tuple,0,ret_objval);
      Store_field (ret_tuple,1,ret_part);
      
      CAMLreturn(ret_tuple);
    }
    else if (ret_val == METIS_ERROR_INPUT) caml_failwith ("METIS FAILED WITH METIS_ERROR_INPUT");
    else if (ret_val == METIS_ERROR_MEMORY) caml_failwith ("METIS FAILED WITH METIS_ERROR_MEMORY");
    else if (ret_val == METIS_ERROR) caml_failwith ("METIS FAILED WITH METIS_ERROR");
  }
  else caml_failwith ("One of the array arguments is incorrect");
}

/*
  These are the mappings 
  argv[0] = v_ne
  argv[1] = v_nn
  argv[2] = v_eptr
  argv[3] = v_eind
  argv[4] = v_vwgt
  argv[5] = v_vsize
  argv[6] = v_ncommon
  argv[7] = v_nparts
  argv[8] = v_tpwgts
 */
value
Metis_PartMeshDualByteCode (value *argv, int argn) {

  CAMLparamN (argv,argn);

  Metis_PartMeshDualNative (argv[0], argv[1], argv[2], argv[3], argv[4],
			    argv[5], argv[6], argv[7], argv[8]);
  /* /\* First get the int values *\/ */
  /* idx_t ne = (idx_t)((Nativeint_val(argv[0]))); */
  /* idx_t nn = (idx_t)((Nativeint_val(argv[1]))); */
  /* idx_t nparts = (idx_t)((Nativeint_val(argv[7]))); */
  /* idx_t ncommon = (idx_t)((Nativeint_val(argv[6]))); */
  
  /* /\* Get all the arrays *\/ */
  /* if(Tag_val(argv[5]) == 0 && Tag_val(argv[2])==0 && Tag_val(argv[3]) ==0 && Tag_val(argv[4])==0){ */
  /*   /\* Use mallocs, because things might overflow!! *\/ */

  /*   /\* Getting xadj *\/ */
  /*   idx_t *vsize = (idx_t*)(malloc((Wosize_val(argv[5])*sizeof(idx_t)))); */
  /*   get_array_values(argv[5],vsize); */

  /*   /\* Getting adjncy *\/ */
  /*   idx_t *eptr = (idx_t*)(malloc((Wosize_val(argv[2])*sizeof(idx_t)))); */
  /*   get_array_values(argv[2],eptr); */

  /*   /\* Getting vwgts *\/ */
  /*   idx_t *eind = (idx_t*)(malloc((Wosize_val(argv[3])*sizeof(idx_t)))); */
  /*   get_array_values(argv[3],eind); */
    
  /*   /\* Getting adjwgt *\/ */
  /*   idx_t *vwgt = (idx_t*)(malloc((Wosize_val(argv[4])*sizeof(idx_t)))); */
  /*   get_array_values(argv[4],vwgt); */
    
  /*   /\* Getting tpwgt *\/ */
  /*   real_t* tpwgts = (real_t*)(malloc((Wosize_val(argv[8])*sizeof(real_t)))); */
  /*   //Check the size */
  /*   if (Wosize_val(argv[8]) != nparts) caml_failwith ("Argument 9 size not equal to arg8"); */
  /*   get_real_array_values(argv[8],tpwgts); */
    
  /*   /\* Now make the return array *\/ */
  /*   idx_t objval; */
    
  /*   /\* Now make the return array *\/ */
  /*   idx_t *epart = (idx_t*)malloc(sizeof(idx_t)*ne); */
  /*   /\* Now make the npart array *\/ */
  /*   idx_t *npart = (idx_t*)malloc(sizeof(idx_t)*nn); */
    
  /*   /\* Now make a call the metis library function *\/ */
  /*   int ret_val = METIS_PartMeshDual(&ne,&nn,eptr,eind,vwgt,vsize,&ncommon,&nparts,tpwgts,NULL, */
  /* 				     &objval,epart,npart); */
  /*   if (ret_val == METIS_OK){ */
  /*     /\*First free all the allocations that we have done, so that there is no memory leak!! *\/ */
  /*     free (epart); */
  /*     free (eind); */
  /*     free (vwgt); */
  /*     free (tpwgts); */

  /*     /\* Now we want to make the tuple that returns the value back in to Ocaml's heap space *\/ */
  /*     /\* DRAGON: This tuple will be garbage collected by the Ocaml GC!! *\/ */
  /*     CAMLlocal4(ret_objval,ret_epart,ret_tuple,ret_npart); */
  /*     ret_objval = caml_copy_nativeint (objval); //The objective value to be sent back */
  /*     ret_epart = caml_alloc (ne,0); //The npart array (snd ret_tuple) */
  /*     ret_npart = caml_alloc (nn,0); //The npart array (snd ret_tuple) */
  /*     ret_tuple = caml_alloc_tuple (3); //The tuple to send back */
      
  /*     /\* Fill in the return array *\/ */
  /*     int i=0; */
  /*     for(i=0;i<ne;++i) */
  /* 	Store_field (ret_epart,i,caml_copy_nativeint(epart[i])); */
  /*     for(i=0;i<nn;++i) */
  /* 	Store_field (ret_npart,i,caml_copy_nativeint(npart[i])); */
      
  /*     free(epart); free(npart); */
  /*     /\* Fill in the tuple for return *\/ */
      
  /*     Store_field (ret_tuple,0,ret_objval); */
  /*     Store_field (ret_tuple,1,ret_epart); */
  /*     Store_field (ret_tuple,2,ret_npart); */
      
  /*     CAMLreturn(ret_tuple); */
  /*   } */
  /*   else if (ret_val == METIS_ERROR_INPUT) caml_failwith ("METIS FAILED WITH METIS_ERROR_INPUT"); */
  /*   else if (ret_val == METIS_ERROR_MEMORY) caml_failwith ("METIS FAILED WITH METIS_ERROR_MEMORY"); */
  /*   else if (ret_val == METIS_ERROR) caml_failwith ("METIS FAILED WITH METIS_ERROR"); */
  /* } */
  /* else caml_failwith ("One of the array arguments is incorrect"); */
}

value
Metis_PartMeshDualNative (value v_ne, value v_nn, value v_eptr, value v_eind,
				value v_vwgt, value v_vsize, value v_ncommon, value v_nparts,
				value v_tpwgts) {

  CAMLparam5(v_ne,v_nn,v_eptr,v_eind,v_vwgt);
  CAMLxparam4(v_vsize,v_ncommon,v_nparts,v_tpwgts);

  /* First get the int values */
  idx_t ne = (idx_t)((Nativeint_val(v_ne)));
  idx_t nn = (idx_t)((Nativeint_val(v_nn)));
  idx_t nparts = (idx_t)((Nativeint_val(v_nparts)));
  idx_t ncommon = (idx_t)((Nativeint_val(v_ncommon)));
  
  /* Get all the arrays */
  if(Tag_val(v_vsize) == 0 && Tag_val(v_eptr)==0 && Tag_val(v_eind) ==0 && Tag_val(v_vwgt)==0){
    /* Use mallocs, because things might overflow!! */

    /* Getting xadj */
    idx_t *vsize = (idx_t*)(malloc((Wosize_val(v_vsize)*sizeof(idx_t))));
    if (Wosize_val(v_vsize) == 0) vsize = NULL;
    else if (Wosize_val(v_vsize) != ne) caml_failwith ("Arg 6 size not equal to arg 1");
    else get_array_values(v_vsize,vsize);

    /* Getting adjncy */
    idx_t *eptr = (idx_t*)(malloc((Wosize_val(v_eptr)*sizeof(idx_t))));
    get_array_values(v_eptr,eptr);

    /* Getting vwgts */
    idx_t *eind = (idx_t*)(malloc((Wosize_val(v_eind)*sizeof(idx_t))));
    get_array_values(v_eind,eind);
    
    /* Getting adjwgt */
    idx_t *vwgt = (idx_t*)(malloc((Wosize_val(v_vwgt)*sizeof(idx_t))));
    if (Wosize_val(v_vwgt) == 0) vsize = NULL;
    else if (Wosize_val(v_vwgt) != ne) caml_failwith ("Arg 5 size not equal to arg 1");
    else get_array_values(v_vwgt,vwgt);
    
    /* Getting tpwgt */
    real_t* tpwgts = (real_t*)(malloc((Wosize_val(v_tpwgts)*sizeof(real_t))));
    //Check the size
    if (Wosize_val(v_tpwgts) == 0) tpwgts = NULL;
    else if (Wosize_val(v_tpwgts) != nparts) caml_failwith ("Argument 9 size not equal to arg8");
    else get_real_array_values(v_tpwgts,tpwgts);
    
    /* Now make the return array */
    idx_t objval;
    
    /* Now make the return array */
    idx_t *epart = (idx_t*)malloc(sizeof(idx_t)*ne);
    /* Now make the npart array */
    idx_t *npart = (idx_t*)malloc(sizeof(idx_t)*nn);
    
    idx_t options [METIS_NOPTIONS];
    if (!set_options) METIS_SetDefaultOptions (options);
    options[METIS_OPTION_DBGLVL] = 1;

    /* Now make a call the metis library function */
    int ret_val = METIS_PartMeshDual(&ne,&nn,eptr,eind,vwgt,vsize,&ncommon,&nparts,tpwgts,NULL,
				     &objval,epart,npart);
    if (ret_val == METIS_OK){
      /*First free all the allocations that we have done, so that there is no memory leak!! */
      free (epart);
      free (eind);
      free (vwgt);
      free (tpwgts);

      /* Now we want to make the tuple that returns the value back in to Ocaml's heap space */
      /* DRAGON: This tuple will be garbage collected by the Ocaml GC!! */
      CAMLlocal4(ret_objval,ret_epart,ret_tuple,ret_npart);
      ret_objval = caml_copy_nativeint (objval); //The objective value to be sent back
      ret_epart = caml_alloc (ne,0); //The npart array (snd ret_tuple)
      ret_npart = caml_alloc (nn,0); //The npart array (snd ret_tuple)
      ret_tuple = caml_alloc_tuple (3); //The tuple to send back
      
      /* Fill in the return array */
      int i=0;
      for(i=0;i<ne;++i)
	Store_field (ret_epart,i,caml_copy_nativeint(epart[i]));
      for(i=0;i<nn;++i)
	Store_field (ret_npart,i,caml_copy_nativeint(npart[i]));
      
      /* Fill in the tuple for return */
      free(epart); free(npart);
      
      Store_field (ret_tuple,0,ret_objval);
      Store_field (ret_tuple,1,ret_epart);
      Store_field (ret_tuple,2,ret_npart);
      
      CAMLreturn(ret_tuple);
    }
    else if (ret_val == METIS_ERROR_INPUT) caml_failwith ("METIS FAILED WITH METIS_ERROR_INPUT");
    else if (ret_val == METIS_ERROR_MEMORY) caml_failwith ("METIS FAILED WITH METIS_ERROR_MEMORY");
    else if (ret_val == METIS_ERROR) caml_failwith ("METIS FAILED WITH METIS_ERROR");
  }
  else caml_failwith ("One of the array arguments is incorrect");
}


value
Metis_PartMeshNodalNative (value v_ne, value v_nn, value v_eptr, value v_eind,
				value v_vwgt, value v_vsize, value v_nparts,
				value v_tpwgts) {

  CAMLparam5(v_ne,v_nn,v_eptr,v_eind,v_vwgt);
  CAMLxparam3(v_vsize,v_nparts,v_tpwgts);

  /* First get the int values */
  idx_t ne = (idx_t)((Nativeint_val(v_ne)));
  idx_t nn = (idx_t)((Nativeint_val(v_nn)));
  idx_t nparts = (idx_t)((Nativeint_val(v_nparts)));
  
  /* Get all the arrays */
  if(Tag_val(v_vsize) == 0 && Tag_val(v_eptr)==0 && Tag_val(v_eind) ==0 && Tag_val(v_vwgt)==0){
    /* Use mallocs, because things might overflow!! */

    /* Getting xadj */
    idx_t *vsize = (idx_t*)(malloc((Wosize_val(v_vsize)*sizeof(idx_t))));
    get_array_values(v_vsize,vsize);

    /* Getting adjncy */
    idx_t *eptr = (idx_t*)(malloc((Wosize_val(v_eptr)*sizeof(idx_t))));
    get_array_values(v_eptr,eptr);

    /* Getting vwgts */
    idx_t *eind = (idx_t*)(malloc((Wosize_val(v_eind)*sizeof(idx_t))));
    get_array_values(v_eind,eind);
    
    /* Getting adjwgt */
    idx_t *vwgt = (idx_t*)(malloc((Wosize_val(v_vwgt)*sizeof(idx_t))));
    if (Wosize_val(v_vwgt) == 0) vwgt = NULL;
    else if (Wosize_val(v_vwgt) != nn) caml_failwith ("Arg 5 not of length arg 2");
    else get_array_values(v_vwgt,vwgt);
    
    /* Getting tpwgt */
    real_t* tpwgts = (real_t*)(malloc((Wosize_val(v_tpwgts)*sizeof(real_t))));
    //Check the size
    if (Wosize_val(v_tpwgts) == 0) tpwgts = NULL;
    else if (Wosize_val(v_tpwgts) != nparts) caml_failwith ("Argument 9 size not equal to arg8");
    else get_real_array_values(v_tpwgts,tpwgts);
    
    /* Now make the return array */
    idx_t objval;
    
    /* Now make the return array */
    idx_t *epart = (idx_t*)malloc(sizeof(idx_t)*ne);
    /* Now make the npart array */
    idx_t *npart = (idx_t*)malloc(sizeof(idx_t)*nn);

    idx_t options[METIS_NOPTIONS];
    if (!set_options) METIS_SetDefaultOptions(options);
    options[METIS_OPTION_DBGLVL] = 1;

    /* Now make a call the metis library function */
    int ret_val = METIS_PartMeshNodal(&ne,&nn,eptr,eind,vwgt,vsize,&nparts,tpwgts,options,
				     &objval,epart,npart);
    if (ret_val == METIS_OK){
      /*First free all the allocations that we have done, so that there is no memory leak!! */
      free (eptr);
      free (eind);
      free (vwgt);
      free (tpwgts);

      /* Now we want to make the tuple that returns the value back in to Ocaml's heap space */
      /* DRAGON: This tuple will be garbage collected by the Ocaml GC!! */
      CAMLlocal4(ret_objval,ret_epart,ret_tuple,ret_npart);
      ret_objval = caml_copy_nativeint (objval); //The objective value to be sent back
      ret_epart = caml_alloc (ne,0); //The npart array (snd ret_tuple)
      ret_npart = caml_alloc (nn,0); //The npart array (snd ret_tuple)
      ret_tuple = caml_alloc_tuple (3); //The tuple to send back
      
      /* Fill in the return array */
      int i=0;
      for(i=0;i<ne;++i)
	Store_field (ret_epart,i,caml_copy_nativeint(epart[i]));
      for(i=0;i<nn;++i)
	Store_field (ret_npart,i,caml_copy_nativeint(npart[i]));

      free(epart); free(npart);
      
      /* Fill in the tuple for return */
      
      Store_field (ret_tuple,0,ret_objval);
      Store_field (ret_tuple,1,ret_epart);
      Store_field (ret_tuple,2,ret_npart);
      
      CAMLreturn(ret_tuple);
    }
    else if (ret_val == METIS_ERROR_INPUT) caml_failwith ("METIS FAILED WITH METIS_ERROR_INPUT");
    else if (ret_val == METIS_ERROR_MEMORY) caml_failwith ("METIS FAILED WITH METIS_ERROR_MEMORY");
    else if (ret_val == METIS_ERROR) caml_failwith ("METIS FAILED WITH METIS_ERROR");
  }
  else caml_failwith ("One of the array arguments is incorrect");
}

/*
  These are the mappings 
  argv[0] = v_ne
  argv[1] = v_nn
  argv[2] = v_eptr
  argv[3] = v_eind
  argv[4] = v_vwgt
  argv[5] = v_vsize
  argv[6] = v_nparts
  argv[7] = v_tpwgts
 */

value
Metis_PartMeshNodalByteCode (value *argv, int argn) {

  CAMLparamN (argv,argn);
  Metis_PartMeshNodalNative (argv[0], argv[1], argv[2], argv[3], argv[4], argv[5],
			     argv[6], argv[7]);

  /* /\* First get the int values *\/ */
  /* idx_t ne = (idx_t)((Nativeint_val(argv[0]))); */
  /* idx_t nn = (idx_t)((Nativeint_val(argv[1]))); */
  /* idx_t nparts = (idx_t)((Nativeint_val(argv[6]))); */
  
  /* /\* Get all the arrays *\/ */
  /* if(Tag_val(argv[5]) == 0 && Tag_val(argv[2])==0 && Tag_val(argv[3]) ==0 && Tag_val(argv[4])==0){ */
  /*   /\* Use mallocs, because things might overflow!! *\/ */

  /*   /\* Getting xadj *\/ */
  /*   idx_t *vsize = (idx_t*)(malloc((Wosize_val(argv[5])*sizeof(idx_t)))); */
  /*   get_array_values(argv[5],vsize); */

  /*   /\* Getting adjncy *\/ */
  /*   idx_t *eptr = (idx_t*)(malloc((Wosize_val(argv[2])*sizeof(idx_t)))); */
  /*   get_array_values(argv[2],eptr); */

  /*   /\* Getting vwgts *\/ */
  /*   idx_t *eind = (idx_t*)(malloc((Wosize_val(argv[3])*sizeof(idx_t)))); */
  /*   get_array_values(argv[3],eind); */
    
  /*   /\* Getting adjwgt *\/ */
  /*   idx_t *vwgt = (idx_t*)(malloc((Wosize_val(argv[4])*sizeof(idx_t)))); */
  /*   get_array_values(argv[4],vwgt); */
    
  /*   /\* Getting tpwgt *\/ */
  /*   real_t* tpwgts = (real_t*)(malloc((Wosize_val(argv[8])*sizeof(real_t)))); */
  /*   //Check the size */
  /*   if (Wosize_val(argv[7]) != nparts) caml_failwith ("Argument 9 size not equal to arg8"); */
  /*   get_real_array_values(argv[7],tpwgts); */
    
  /*   /\* Now make the return array *\/ */
  /*   idx_t objval; */
    
  /*   /\* Now make the return array *\/ */
  /*   idx_t *epart = (idx_t*)malloc(sizeof(idx_t)*ne); */
  /*   /\* Now make the npart array *\/ */
  /*   idx_t *npart = (idx_t*)malloc(sizeof(idx_t)*nn); */
    
  /*   /\* Now make a call the metis library function *\/ */
  /*   int ret_val = METIS_PartMeshNodal(&ne,&nn,eptr,eind,vwgt,vsize,&nparts,tpwgts,NULL, */
  /* 				     &objval,epart,npart); */
  /*   if (ret_val == METIS_OK){ */
  /*     /\*First free all the allocations that we have done, so that there is no memory leak!! *\/ */
  /*     free (eptr); */
  /*     free (eind); */
  /*     free (vwgt); */
  /*     free (tpwgts); */

  /*     /\* Now we want to make the tuple that returns the value back in to Ocaml's heap space *\/ */
  /*     /\* DRAGON: This tuple will be garbage collected by the Ocaml GC!! *\/ */
  /*     CAMLlocal4(ret_objval,ret_epart,ret_tuple,ret_npart); */
  /*     ret_objval = caml_copy_nativeint (objval); //The objective value to be sent back */
  /*     ret_epart = caml_alloc (ne,0); //The npart array (snd ret_tuple) */
  /*     ret_npart = caml_alloc (nn,0); //The npart array (snd ret_tuple) */
  /*     ret_tuple = caml_alloc_tuple (3); //The tuple to send back */
      
  /*     /\* Fill in the return array *\/ */
  /*     int i=0; */
  /*     for(i=0;i<ne;++i) */
  /* 	Store_field (ret_epart,i,caml_copy_nativeint(epart[i])); */
  /*     for(i=0;i<nn;++i) */
  /* 	Store_field (ret_npart,i,caml_copy_nativeint(npart[i])); */
      
  /*     free(epart); free(npart); */
  /*     /\* Fill in the tuple for return *\/ */
      
  /*     Store_field (ret_tuple,0,ret_objval); */
  /*     Store_field (ret_tuple,1,ret_epart); */
  /*     Store_field (ret_tuple,2,ret_npart); */
      
  /*     CAMLreturn(ret_tuple); */
  /*   } */
  /*   else if (ret_val == METIS_ERROR_INPUT) caml_failwith ("METIS FAILED WITH METIS_ERROR_INPUT"); */
  /*   else if (ret_val == METIS_ERROR_MEMORY) caml_failwith ("METIS FAILED WITH METIS_ERROR_MEMORY"); */
  /*   else if (ret_val == METIS_ERROR) caml_failwith ("METIS FAILED WITH METIS_ERROR"); */
  /* } */
  /* else caml_failwith ("One of the array arguments is incorrect"); */
}

value
Metis_NodeND (value v_nvtx, value v_xadj, value v_adjncy, value v_vwgts){
  CAMLparam4 (v_nvtx,v_xadj,v_adjncy,v_vwgts);
  idx_t nvtxs = (idx_t)((Nativeint_val(v_nvtx)));
  if(Tag_val(v_xadj) == 0 && Tag_val(v_adjncy)==0 && Tag_val(v_vwgts) ==0 ){
    /* Getting xadj */
    idx_t *xadj = (idx_t*)(malloc((Wosize_val(v_xadj)*sizeof(idx_t))));
    get_array_values(v_xadj,xadj);
    /* Getting adjncy */
    idx_t *adjncy = (idx_t*)(malloc((Wosize_val(v_adjncy)*sizeof(idx_t))));
    get_array_values(v_adjncy,adjncy);
    /* Getting vwgts */
    idx_t *vwgts = (idx_t*)(malloc((Wosize_val(v_vwgts)*sizeof(idx_t))));
    if (Wosize_val(v_vwgts) == 0) vwgts = NULL;
    else if (Wosize_val(v_vwgts) != nvtxs) caml_failwith ("Arg 4 not of length arg 1");
    else get_array_values(v_vwgts,vwgts);
    
    idx_t *perm = (idx_t*)malloc(nvtxs*sizeof(idx_t));
    idx_t *iperm = (idx_t*)malloc(nvtxs*sizeof(idx_t));

    idx_t options[METIS_NOPTIONS];
    if (!set_options)METIS_SetDefaultOptions(options);
    options[METIS_OPTION_DBGLVL] = 1;
    
    int ret_val = METIS_NodeND (&nvtxs,xadj,adjncy,vwgts,options,perm,iperm);
    if (ret_val == METIS_OK){
      
      free (xadj);
      free (adjncy);
      free (vwgts);

      CAMLlocal3(ret_perm,ret_iperm,ret_tuple);
      ret_perm = caml_alloc (nvtxs,0); //The npart array (snd ret_tuple)
      ret_iperm = caml_alloc (nvtxs,0); //The npart array (snd ret_tuple)
      ret_tuple = caml_alloc_tuple (2); //The tuple to send back
      /* Fill in the return array */
      int i=0;
      for(i=0;i<nvtxs;++i)
	Store_field (ret_perm,i,caml_copy_nativeint(perm[i]));
      for(i=0;i<nvtxs;++i)
	Store_field (ret_iperm,i,caml_copy_nativeint(iperm[i]));
      
      free(perm); free(iperm);
      
      /* Fill in the tuple for return */
      Store_field (ret_tuple,0,ret_perm);
      Store_field (ret_tuple,1,ret_iperm);
      
      CAMLreturn(ret_tuple);
    }
    else if (ret_val == METIS_ERROR_INPUT) caml_failwith ("METIS FAILED WITH METIS_ERROR_INPUT");
    else if (ret_val == METIS_ERROR_MEMORY) caml_failwith ("METIS FAILED WITH METIS_ERROR_MEMORY");
    else if (ret_val == METIS_ERROR) caml_failwith ("METIS FAILED WITH METIS_ERROR");
  }
}
