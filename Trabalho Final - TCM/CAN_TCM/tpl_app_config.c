/**
 * @file tpl_app_config.c
 *
 * @section desc File description
 *
 * OS data structure generated from application test
 * Automatically generated by goil on Mon Jun 12 17:13:03 2023
 * from root OIL file CAN_TCM.oil
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005-2007
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include <stdbool.h>
#include <stdint.h>

#include "tpl_app_config.h"

#include "tpl_os_internal_types.h"
#include "tpl_machine.h"
#include "tpl_os_interrupt.h"
#include "tpl_os_interrupt_kernel.h"
#include "tpl_os_alarm_kernel.h"
#include "tpl_os_alarm.h"
#include "tpl_os_resource_kernel.h"
#include "tpl_os_resource.h"
#include "tpl_os_event_kernel.h"
#include "tpl_os_event.h"
#include "tpl_os_action.h"
#include "tpl_os_kernel.h"
#include "tpl_os_definitions.h"


/*=============================================================================
 * Application Modes tables for tasks, alarms and schedule tables
 * Application Modes masks are as follow:
 * stdAppmode = 1
 */

CONST(tpl_application_mode, OS_CONST) stdAppmode = 0; /* mask = 1 */
CONST(tpl_application_mode, OS_CONST) OSDEFAULTAPPMODE = 0;
CONST(tpl_appmode_mask, OS_CONST) tpl_task_app_mode[TASK_COUNT] = {
  0 /* task ReceberMarchaAtual :  */ ,
  0 /* task SendCANM1 :  */ 
};

CONST(tpl_appmode_mask, OS_CONST) tpl_alarm_app_mode[ALARM_COUNT] = {
  1 /* alarm periodicAlarm1 : stdAppmode */ ,
  1 /* alarm periodicAlarm2 : stdAppmode */ 
};

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of resources IDs
 */

/* Resource res1 */
#define res1_id 0
CONST(ResourceType, AUTOMATIC) res1 = res1_id;

/*=============================================================================
 * Declaration of processes IDs
 */

/* Task ReceberMarchaAtual identifier */
#define ReceberMarchaAtual_id 0
CONST(TaskType, AUTOMATIC) ReceberMarchaAtual = ReceberMarchaAtual_id;

/* Task SendCANM1 identifier */
#define SendCANM1_id 1
CONST(TaskType, AUTOMATIC) SendCANM1 = SendCANM1_id;

/*=============================================================================
 * Declaration of Alarm IDs
 */

/* Alarm periodicAlarm1 identifier */
#define periodicAlarm1_id 0
CONST(AlarmType, AUTOMATIC) periodicAlarm1 = periodicAlarm1_id;

/* Alarm periodicAlarm2 identifier */
#define periodicAlarm2_id 1
CONST(AlarmType, AUTOMATIC) periodicAlarm2 = periodicAlarm2_id;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Definition and initialization of regular Resource related structures
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*-----------------------------------------------------------------------------
 * Resource descriptor of resource res1
 *
 */
VAR(tpl_resource, OS_VAR) res1_rez_desc = {
  /* ceiling priority of the resource */  1,
  /* owner previous priority          */  0,
  /* owner of the resource            */  INVALID_PROC_ID,
#if WITH_OSAPPLICATION == YES
  /* OS Application id                */  
#endif

  /* next resource in the list        */  NULL
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/**
 * The scheduler resource descriptor.
 * One scheduler resource is defined per core.
 *
 * @see #RES_SCHEDULER
 */

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
VAR(tpl_resource, OS_VAR) res_sched_rez_desc = {
  RES_SCHEDULER_PRIORITY,   /*  ceiling priority                            */
  0,                        /*  owner_prev_priority                         */
  INVALID_PROC_ID,          /*  owner                                       */
#if WITH_OSAPPLICATION == YES
  INVALID_OSAPPLICATION_ID, /*  OS Application id                           */
#endif

  NULL                      /*  next_res                                    */
};
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_resource, AUTOMATIC, OS_APPL_DATA)
tpl_resource_table[RESOURCE_COUNT] = {
  &res1_rez_desc,
  &res_sched_rez_desc
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Definition and initialization of Counters related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Counter SystemCounter descriptor
 */

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_tick, OS_CONST) OSTICKSPERBASE = 1;
CONST(tpl_tick, OS_CONST) OSMAXALLOWEDVALUE = 65535;
CONST(tpl_tick, OS_CONST) OSMINCYCLE = 1;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_counter, OS_VAR) SystemCounter_counter_desc = {
  /* ticks per base       */  1,
  /* max allowed value    */  65535,
  /* minimum cycle        */  1,
  /* current tick         */  0,
  /* current date         */  0,
#if WITH_OSAPPLICATION == YES
    /* OS Application id    */  
#endif
    /* first alarm          */  NULL_PTR,
    /* next alarm to raise  */  NULL_PTR
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#include "tpl_os_kernel.h"          /* tpl_schedule */
#include "tpl_os_timeobj_kernel.h"  /* tpl_counter_tick */
#include "tpl_machine_interface.h"  /* tpl_switch_context_from_it */

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

//gcc uses ISR as a keyword to define an interrupt handler.
//Osek uses ISR to define an ISR2 :-/
#ifdef ISR
 #undef ISR
#endif
#include <avr/interrupt.h>

//function defined in avr assembly files.
FUNC(void, OS_CODE) tpl_avr_counter_tick(
    P2VAR(tpl_counter, AUTOMATIC, OS_APPL_DATA) counter);


void trampolineSystemCounter()
{
  tpl_avr_counter_tick(&SystemCounter_counter_desc);
}
  
#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/*=============================================================================
 * Descriptor(s) of the idle task(s)
 */
/*-----------------------------------------------------------------------------
 * Task IDLE_TASK descriptor
 */
#define OS_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * Task IDLE_TASK function prototype
 */

FUNC(void, OS_APPL_CODE) idle_function(void);
#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */

/*
 * Task IDLE_TASK stack
 *
 */
#define APP_Task_IDLE_TASK_START_SEC_STACK
#include "tpl_memmap.h"
tpl_stack_word IDLE_TASK_stack_zone[IDLE_STACK_SIZE/sizeof(tpl_stack_word)];
#define APP_Task_IDLE_TASK_STOP_SEC_STACK
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define IDLE_TASK_STACK { IDLE_TASK_stack_zone, IDLE_STACK_SIZE }

/*
 * Task IDLE_TASK context
 */
avr_context IDLE_TASK_int_context;
#define IDLE_TASK_CONTEXT &IDLE_TASK_int_context

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"




/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task IDLE_TASK
 */
CONST(tpl_proc_static, OS_CONST) IDLE_TASK_task_stat_desc = {
  /* context                  */  IDLE_TASK_CONTEXT,
  /* stack                    */  IDLE_TASK_STACK,
  /* entry point (function)   */  idle_function,
  /* internal ressource       */  NULL,
  /* task id                  */  IDLE_TASK_ID,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  0,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task IDLE_TASK
 */
VAR(tpl_proc, OS_VAR) IDLE_TASK_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  0,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Task related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Task ReceberMarchaAtual descriptor
 */
#define APP_Task_ReceberMarchaAtual_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task ReceberMarchaAtual function prototype
 */

FUNC(void, OS_APPL_CODE) ReceberMarchaAtual_function(void);
#define APP_Task_ReceberMarchaAtual_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */

/*
 * Task ReceberMarchaAtual stack
 *
 */
#define APP_Task_ReceberMarchaAtual_START_SEC_STACK
#include "tpl_memmap.h"
tpl_stack_word ReceberMarchaAtual_stack_zone[256/sizeof(tpl_stack_word)];
#define APP_Task_ReceberMarchaAtual_STOP_SEC_STACK
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define ReceberMarchaAtual_STACK { ReceberMarchaAtual_stack_zone, 256 }

/*
 * Task ReceberMarchaAtual context
 */
avr_context ReceberMarchaAtual_int_context;
#define ReceberMarchaAtual_CONTEXT &ReceberMarchaAtual_int_context

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"




/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task ReceberMarchaAtual
 */
CONST(tpl_proc_static, OS_CONST) ReceberMarchaAtual_task_stat_desc = {
  /* context                  */  ReceberMarchaAtual_CONTEXT,
  /* stack                    */  ReceberMarchaAtual_STACK,
  /* entry point (function)   */  ReceberMarchaAtual_function,
  /* internal ressource       */  NULL,
  /* task id                  */  ReceberMarchaAtual_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  2,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task ReceberMarchaAtual
 */
VAR(tpl_proc, OS_VAR) ReceberMarchaAtual_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  2,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Task SendCANM1 descriptor
 */
#define APP_Task_SendCANM1_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task SendCANM1 function prototype
 */

FUNC(void, OS_APPL_CODE) SendCANM1_function(void);
#define APP_Task_SendCANM1_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */

/*
 * Task SendCANM1 stack
 *
 */
#define APP_Task_SendCANM1_START_SEC_STACK
#include "tpl_memmap.h"
tpl_stack_word SendCANM1_stack_zone[256/sizeof(tpl_stack_word)];
#define APP_Task_SendCANM1_STOP_SEC_STACK
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define SendCANM1_STACK { SendCANM1_stack_zone, 256 }

/*
 * Task SendCANM1 context
 */
avr_context SendCANM1_int_context;
#define SendCANM1_CONTEXT &SendCANM1_int_context

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"




/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task SendCANM1
 */
CONST(tpl_proc_static, OS_CONST) SendCANM1_task_stat_desc = {
  /* context                  */  SendCANM1_CONTEXT,
  /* stack                    */  SendCANM1_STACK,
  /* entry point (function)   */  SendCANM1_function,
  /* internal ressource       */  NULL,
  /* task id                  */  SendCANM1_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  3,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task SendCANM1
 */
VAR(tpl_proc, OS_VAR) SendCANM1_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  3,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


#include "tpl_machine.h"
#define OS_START_SEC_CODE
#include "tpl_memmap.h"

//function defined in avr assembly files.
FUNC(void, OS_CODE) tpl_avr_ISR2_handler(CONST(uint16, AUTOMATIC) id);


#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
/*=============================================================================
 * Definition and initialization of process tables (tasks and isrs)
 */
CONSTP2CONST(tpl_proc_static, AUTOMATIC, OS_APPL_DATA)
tpl_stat_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &ReceberMarchaAtual_task_stat_desc,
  &SendCANM1_task_stat_desc,
  &IDLE_TASK_task_stat_desc
};

CONSTP2VAR(tpl_proc, AUTOMATIC, OS_APPL_DATA)
tpl_dyn_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &ReceberMarchaAtual_task_desc,
  &SendCANM1_task_desc,
  &IDLE_TASK_task_desc
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Alarm related structures
 */
/*-----------------------------------------------------------------------------
 * Alarm periodicAlarm1 descriptor
 *
 * This alarm does the activation of task ReceberMarchaAtual
 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_task_activation_action, OS_CONST) periodicAlarm1_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  ReceberMarchaAtual_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) periodicAlarm1_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_ID == YES)
    /* id of the alarm for tracing  */  , periodicAlarm1_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&periodicAlarm1_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) periodicAlarm1_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&periodicAlarm1_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  70,
    /* date                         */  400,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Alarm periodicAlarm2 descriptor
 *
 * This alarm does the activation of task SendCANM1
 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_task_activation_action, OS_CONST) periodicAlarm2_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  SendCANM1_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) periodicAlarm2_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_ID == YES)
    /* id of the alarm for tracing  */  , periodicAlarm2_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&periodicAlarm2_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) periodicAlarm2_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&periodicAlarm2_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  110,
    /* date                         */  600,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_time_obj, AUTOMATIC, OS_APPL_DATA)
  tpl_alarm_table[ALARM_COUNT] = {
  &periodicAlarm1_alarm_desc,
  &periodicAlarm2_alarm_desc
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of flags functions
 */
/* $FLAGSFUNCTIONS$ */

/*=============================================================================
 * Definition and initialization of Ready List structures
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_heap_entry, OS_VAR) tpl_ready_list[5];
VAR(tpl_rank_count, OS_VAR) tpl_tail_for_prio[5] = {
  0,
  0,
  0,
  0
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/**
 * @internal
 *
 * tpl_kern gather informations on the current executing object and
 * the previous one
 */
VAR(tpl_kern_state, OS_VAR) tpl_kern =
{
  NULL,                      /* no running task static descriptor   */
  &IDLE_TASK_task_stat_desc, /* elected task to run is idle task    */
  NULL,                      /* no running task dynamic descriptor  */
  &IDLE_TASK_task_desc,      /* elected task to run is idle task    */
  INVALID_PROC_ID,           /* no running task so no ID            */
  INVALID_PROC_ID,           /* idle task has no ID                 */
  NO_NEED_SWITCH,            /* no context switch needed at start   */
  FALSE,                     /* no schedule needed at start         */
#if WITH_MEMORY_PROTECTION == YES
  1,                         /* at early system startup, we run in  */
                             /*  kernel mode, so in trusted mode    */
#endif /* WITH_MEMORY_PROTECTION */
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2CONST(char, AUTOMATIC, OS_APPL_DATA) proc_name_table[TASK_COUNT + ISR_COUNT + 1] = {

  "ReceberMarchaAtual",
  "SendCANM1",
  "*idle*"
};
#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/* End of file tpl_app_config.c */
