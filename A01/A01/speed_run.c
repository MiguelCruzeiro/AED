//
// AED, August 2022 (Tomás Oliveira e Silva)
//
// First practical assignement (speed run)
//
// Compile using either
//   cc -Wall -O2 -D_use_zlib_=0 solution_speed_run.c -lm
// or
//   cc -Wall -O2 -D_use_zlib_=1 solution_speed_run.c -lm -lz
//
// Place your student numbers and names here
//   N.Mec. XXXXXX  Name: XXXXXXX
//

//
// static configuration
//

#define _max_road_size_ 800 // the maximum problem size
#define _min_road_speed_ 2  // must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_ 9  // must not be larger than 9 (only because of the PDF figure)

//#define sol_Prof
//#define sol_Prof_c_array
#define sol_Prof_c_array_acel_first
//#define sol_4

//
// include files --- as this is a small project, we include the PDF generation code directly from make_custom_pdf.c
//

#include <math.h>
#include <stdio.h>
#include "elapsed_time.h"
#include "make_custom_pdf.c"

//
// road stuff
//

static int max_road_speed[1 + _max_road_size_]; // positions 0.._max_road_size_

static void init_road_speeds(void)
{
  double speed;
  int i;

  for (i = 0; i <= _max_road_size_; i++)
  {
    speed = (double)_max_road_speed_ * (0.55 + 0.30 * sin(0.11 * (double)i) + 0.10 * sin(0.17 * (double)i + 1.0) + 0.15 * sin(0.19 * (double)i));
    max_road_speed[i] = (int)floor(0.5 + speed) + (int)((unsigned int)random() % 3u) - 1;
    if (max_road_speed[i] < _min_road_speed_)
      max_road_speed[i] = _min_road_speed_;
    if (max_road_speed[i] > _max_road_speed_)
      max_road_speed[i] = _max_road_speed_;
  }
}

//
// description of a solution
//

typedef struct
{
  int n_moves;                        // the number of moves (the number of positions is one more than the number of moves)
  int positions[1 + _max_road_size_]; // the positions (the first one must be zero)
} solution_t;

//
// the (very inefficient) recursive solution given to the students
//

static solution_t solution_1, solution_1_best;
static double solution_1_elapsed_time; // time it took to solve the problem
static unsigned long solution_1_count; // effort dispended solving the problem
static int pos_spe[_max_road_size_];

static solution_t solution_2, solution_2_best;
static double solution_2_elapsed_time; // time it took to solve the problem
static unsigned long solution_2_count; // effort dispended solving the problem
static int best_move_number_2[_max_road_size_][_max_road_speed_];

static solution_t solution_3, solution_3_best;
static double solution_3_elapsed_time; // time it took to solve the problem
static unsigned long solution_3_count; // effort dispended solving the problem
static int best_move_number_3[_max_road_size_+1][_max_road_speed_+1];
static long counter_sol_3 = 0;
static long counter_sol_final_3 = 0;
static int positions3[_max_road_size_+1] = {-1};

static solution_t solution_4, solution_4_best;
static double solution_4_elapsed_time; // time it took to solve the problem
static unsigned long solution_4_count; // effort dispended solving the problem
static int best_move_number_4[_max_road_size_+1][_max_road_speed_+1];
static long counter_sol_4 = 0;
static long counter_sol_final_4 = 0;




static solution_t solution_5, solution_5_best;
static double solution_5_elapsed_time; // time it took to solve the problem
static unsigned long solution_5_count; // effort dispended solving the problem
static int best_move_number_5[_max_road_size_+1] ;

static int solution_1_recursion(int move_number, int position, int speed, int positions1[])
{
  int new_speed, i;
  solution_1_count++;
  solution_1.positions[move_number] = position;
  positions1[position] = move_number;
  if (positions1[800] == 1 && position == 800)
    return;
  else if (positions1[position] > move_number)
    return;
  else
  
  for (new_speed = speed - 1; new_speed <= speed + 1; new_speed++)
    if (new_speed >= 1 && new_speed <= _max_road_speed_ && position - new_speed >= 0)
    {
      for (i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++)
        ;
      if (i > new_speed)
      { 
        positions1[position] = solution_1_recursion(position + new_speed, new_speed, move_number + 1, positions1);
        
      }
    }

}



static void solution_2_recursion(int move_number, int position, int speed, int final_position)
{
  int i, new_speed;
  solution_2_count++;
  solution_2.positions[move_number] = position;

  if (position == final_position && speed == 1)
  {
    if (move_number < solution_2_best.n_moves)
    {
      solution_2_best = solution_2;
      best_move_number_2[position][speed] = move_number;
      solution_2_best.n_moves = move_number;
    }
    return;
  }
  if (move_number > best_move_number_2[position][speed] && best_move_number_2[position][speed] != 0)
    return;
  for (new_speed = speed - 1; new_speed <= speed + 1; new_speed++)
    if (new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for (i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++)
        ;
      if (i > new_speed)
        solution_2_recursion(move_number + 1, position + new_speed, new_speed, final_position);
    }
}

static void solution_3_recursion(int move_number, int position, int speed, int final_position)
{
  int i, new_speed;
  solution_3_count++;
  solution_3.positions[move_number] = position;

  if (position == final_position && speed == 1)
  {
    if (move_number < solution_3_best.n_moves)
    {
      solution_3_best = solution_3;
      best_move_number_3[position][speed] = move_number;
      counter_sol_final_3 += 1;
      solution_3_best.n_moves = move_number;
    }
    return;
  }
  if (move_number == best_move_number_3[position][speed]){
    counter_sol_3 +=1;
  }
  if (move_number >= best_move_number_3[position][speed]){
    return;
  }
  best_move_number_3[position][speed]=move_number;
  for (new_speed = speed + 1; new_speed >= speed - 1; new_speed--)      
  
  
    if (new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      if (best_move_number_3[position+new_speed][new_speed] <= move_number+1){
      
      return;
    }else{
      
      for (i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++)
        ;
      {
        if (i > new_speed)
          {
              
                solution_3_recursion(move_number + 1, position + new_speed, new_speed, final_position);
              
        }
      }
    }
  }
}

static void solution_4_recursion(int move_number, int position, int speed, int final_position)
{
  int i, new_speed;
  solution_4_count++;
  solution_4.positions[move_number] = position;

  if (position == final_position && speed == 1)
  {
    if (move_number < solution_4_best.n_moves)
    {
      solution_4_best = solution_4;
      best_move_number_4[position][speed] = move_number;
      counter_sol_final_4 += 1;
      solution_4_best.n_moves = move_number;
    }
    return;
  }
  if (best_move_number_4[position][speed] != 0){
    return;
  }
  best_move_number_4[position][speed]=move_number;
  
  for (new_speed = speed + 1; new_speed >= speed - 1; new_speed--) 
  if (best_move_number_4[position+new_speed][new_speed] == 0){
    if (new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for (i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++)
        ;
      {
        if (i > new_speed)
          {
            
              solution_4_recursion(move_number + 1, position + new_speed, new_speed, final_position);
            
            
        }
      }
    }
  }
}

// tentar fazer a 4 solução com um array random[position_anterior] e funçao separada para o anterior


static void solve_1(int final_position)
{
  if (final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr, "solve_1: bad final_position\n");
    exit(1);
  }
  solution_1_elapsed_time = cpu_time();
  solution_1_count = 0ul;

  solution_1_best.n_moves = final_position + 100;
  int positions1[ _max_road_size_] = {0};
  positions1[0] = 1;
  solution_1_recursion(1, 1, 0, positions1);
 
  solution_1_elapsed_time = cpu_time() - solution_1_elapsed_time;
}

static void solve_2(int final_position)
{
  if (final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr, "solve_2: bad final_position\n");
    exit(1);
  }
  solution_2_elapsed_time = cpu_time();
  solution_2_count = 0ul;

  solution_2_best.n_moves = final_position + 100;
  solution_2_recursion(0, 0, 0, final_position);
  solution_2_elapsed_time = cpu_time() - solution_2_elapsed_time;
}

static void solve_3(int final_position)
{
  if (final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr, "solve_3: bad final_position\n");
    exit(1);
  }
  solution_3_elapsed_time = cpu_time();
  solution_3_count = 0ul;
  for (int b = 0 ;b<=_max_road_size_;b++)
  {
    for (int f=0; f<=_max_road_speed_;f++)
      best_move_number_3[b][f]=10000;
  }
  solution_3_best.n_moves = final_position + 100;
  solution_3_recursion(0, 0, 0, final_position);
  solution_3_elapsed_time = cpu_time() - solution_3_elapsed_time;
}

static void solve_4(int final_position)
{
  if (final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr, "solve_4: bad final_position\n");
    exit(1);
  }
  solution_4_elapsed_time = cpu_time();
  solution_4_count = 0ul;
  for (int b = 0 ;b<=_max_road_size_;b++)
  {
    for (int f=0; f<=_max_road_speed_;f++)
      best_move_number_4[b][f]=0;
  }
  solution_4_best.n_moves = final_position + 100;
  solution_4_recursion(0, 0, 0, final_position);
  solution_4_elapsed_time = cpu_time() - solution_4_elapsed_time;
}

//
// example of the slides
//
static void example(void)
{
  int i, final_position;

  srandom(0xAED2022);
  init_road_speeds();
  final_position = 30;
  solve_3(final_position);
  make_custom_pdf_file("example.pdf", final_position, &max_road_speed[0], solution_3_best.n_moves, &solution_3_best.positions[0], solution_3_elapsed_time, solution_3_count, "Plain recursion");
  printf("mad road speeds:");
  for (i = 0; i <= final_position; i++)
    printf(" %d", max_road_speed[i]);
  printf("\n");
  printf("positions:");
  for (i = 0; i <= solution_3_best.n_moves; i++)
    printf(" %d", solution_3_best.positions[i]);
  printf("\n");
}

//
// main program
//

int main(int argc, char *argv[argc + 1])
{
#define _time_limit_ 3600.0
  int n_mec, final_position, print_this_one;
  char file_name[64];

  // generate the example data
  if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'e' && argv[1][2] == 'x')
  {
    example();
    return 0;
  }
  // initialization
  n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
  srandom((unsigned int)n_mec);
  init_road_speeds();
  // run all solution methods for all interesting sizes of the problem
  final_position = 1;
  solution_1_elapsed_time = 0.0;
  solution_2_elapsed_time = 0.0;
  solution_3_elapsed_time = 0.0;
  solution_4_elapsed_time = 0.0;
  printf("    + --- ---------------- --------- +\n");
  printf("    |                plain recursion |\n");
  printf("--- + --- ---------------- --------- +\n");
  printf("  n | sol            count  cpu time |\n");
  printf("--- + --- ---------------- --------- +\n");
  while (final_position <= _max_road_size_ /* && final_position <= 20*/)
  {
    print_this_one = (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
    printf("%3d |", final_position);

// first solution method (very bad)
#ifdef sol_Prof
    if (solution_1_elapsed_time < _time_limit_)
    {
      solve_1(final_position);
      if (print_this_one != 0)
      {
        sprintf(file_name, "%03d_1.pdf", final_position);
        make_custom_pdf_file(file_name, final_position, &max_road_speed[0], solution_1_best.n_moves, &solution_1_best.positions[0], solution_1_elapsed_time, solution_1_count, "Plain recursion");
      }
      printf(" %3d %16lu %9.3e |", solution_1_best.n_moves, solution_1_count, solution_1_elapsed_time);
    }
    else
    {
      solution_1_best.n_moves = -1;
      printf("                                |");
    }
#endif

// second solution method (little better than the first one))
#ifdef sol_Prof_c_array
    if (solution_2_elapsed_time < _time_limit_)
    {
      solve_2(final_position);
      if (print_this_one != 0)
      {
        sprintf(file_name, "%03d_2.pdf", final_position);
        make_custom_pdf_file(file_name, final_position, &max_road_speed[0], solution_2_best.n_moves, &solution_2_best.positions[0], solution_2_elapsed_time, solution_2_count, "Plain recursion");
      }
      printf(" %3d %16lu %9.3e |", solution_2_best.n_moves, solution_2_count, solution_2_elapsed_time);
    }
    else
    {
      solution_2_best.n_moves = -1;
      printf("                                |");
    }
#endif
// third solution method (little worst than second one)
#ifdef sol_Prof_c_array_acel_first
    if (solution_3_elapsed_time < _time_limit_)
    {
      solve_3(final_position);
      if (print_this_one != 0)
      {
        sprintf(file_name, "%03d_3.pdf", final_position);
        make_custom_pdf_file(file_name, final_position, &max_road_speed[0], solution_3_best.n_moves, &solution_3_best.positions[0], solution_3_elapsed_time, solution_3_count, "Plain v3 recursion");
      }
      printf(" %3d %16lu %9.3e %5d %3d |", solution_3_best.n_moves, solution_3_count, solution_3_elapsed_time,counter_sol_3,counter_sol_final_3);
    }
    else
    {
      solution_3_best.n_moves = -1;
      printf("                                |");
    }
#endif

#ifdef sol_4
    if (solution_4_elapsed_time < _time_limit_)
    {
      solve_4(final_position);
      if (print_this_one != 0)
      {
        sprintf(file_name, "%03d_4.pdf", final_position);
        make_custom_pdf_file(file_name, final_position, &max_road_speed[0], solution_4_best.n_moves, &solution_4_best.positions[0], solution_4_elapsed_time, solution_4_count, "Plain v4 recursion");
      }
      printf(" %3d %16lu %9.3e %6d %5d |", solution_4_best.n_moves, solution_4_count, solution_4_elapsed_time,counter_sol_4,counter_sol_final_4);
    }
    else
    {
      solution_4_best.n_moves = -1;
      printf("                                |");
    }
#endif
    // done
    printf("\n");
    fflush(stdout);
    // new final_position
    if (final_position < 50)
      final_position += 1;
    else if (final_position < 100)
      final_position += 5;
    else if (final_position < 200)
      final_position += 10;
    else
      final_position += 20;
  }
  printf("--- + --- ---------------- --------- + --- ---------------- --------- +\n");
  return 0;
#undef _time_limit_
}