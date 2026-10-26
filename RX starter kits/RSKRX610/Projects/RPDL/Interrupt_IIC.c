/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name    : Interrupt_IIC.c
* Version    : 0.10
* Contents    : Interrupt handlers for all IIC channels
* Customer    : 
* Model      : 
* Order      : 
* CPU      : RX600
* Compiler    : RXC
* OS      : Nothing
* Programmer  : 
* Note      : 
************************************************************************
* Copyright,2010 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2010.01.04
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_iic.h"
#include "r_pdl_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline: IICn event interrupt processing
*-------------------------------------------------------------------
* Declaration  : void InterruptIIC_ICEEIn(void)
*-------------------------------------------------------------------
* Function    : 
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Output    : 
*-------------------------------------------------------------------
* Use function  : 
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2010.03.02
*        : Ver 0.10
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ICEEI0
__fast_interrupt void Interrupt_IIC_ICEEI0(void)
#else
#pragma vector = PDL_INTC_VECTOR_ICEEI0
__interrupt void Interrupt_IIC_ICEEI0(void)
#endif
{
  uint8_t valid_flags;
  volatile uint8_t byte;
  
  /* Read the status register */
  valid_flags = RIIC0.ICSR2.BYTE;

  /* Remove any that are not enabled */
  valid_flags &= RIIC0.ICIER.BYTE;

  /* Remove the transmit and receive flags */
  valid_flags &= 0x1Fu;

  /* Start detected? */
  if ((valid_flags & BIT_2) != 0x0u)
  {
    /* Disable Start interrupt request generation */
    RIIC0.ICIER.BIT.STIE = 0;
    /* Decide what to send */
    switch(rpdl_IIC_next_state[0])
    {
      case MASTER_SEND_SLAVE_ADDRESS_7:
        /* Send the slave address */
        RIIC0.ICDRT = rpdl_IIC_slave_address_lower[0];

        /* Transmit mode? */
        if ((rpdl_IIC_slave_address_lower[0] & BIT_0) == 0)
        {
          rpdl_IIC_current_state[0] = MASTER_SEND_DATA;
        }
        else
        {
          rpdl_IIC_current_state[0] = MASTER_START_READ;
        }
        break;
      case MASTER_SEND_SLAVE_ADDRESS_10a:
        rpdl_IIC_current_state[0] = MASTER_SEND_SLAVE_ADDRESS_10b;

        /* Send the first part of the slave address */
        RIIC0.ICDRT = rpdl_IIC_slave_address_upper[0];
        break;
      default:
        break;
    }
  }
  /* NACK detected? */
  else if ((valid_flags & BIT_4) != 0x0u)
  {
    /* Disable NACK interrupt request generation */
    RIIC0.ICIER.BIT.NAKIE = 0;

    /* Failed transmission of data? */
    if (rpdl_IIC_current_state[0] == MASTER_SEND_DATA)
    {
      /* Decrement the counter */
      rpdl_IIC_tx_counter[0]--;

      /* Call the callback function */
      if (rpdl_IIC_callback_func[0] != PDL_NO_FUNC)
      {
        rpdl_IIC_callback_func[0]();
      }
    }
    /* NACK received from the master? */
    else if (rpdl_IIC_current_state[0] == SLAVE_SEND_DATA)
    {
      /* Do a dummy read to release SCL */
      byte = RIIC0.ICDRR;
    }
  }
  else
  {
    /* Disable all interrupt request generation */
    RIIC0.ICIER.BYTE = 0x00u;
      
    /* Call the callback function */
    if (rpdl_IIC_callback_func[0] != PDL_NO_FUNC)
    {
      rpdl_IIC_callback_func[0]();
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ICEEI1
__fast_interrupt void Interrupt_IIC_ICEEI1(void)
#else
#pragma vector = PDL_INTC_VECTOR_ICEEI1
__interrupt void Interrupt_IIC_ICEEI1(void)
#endif
{
  uint8_t valid_flags;
  volatile uint8_t byte;
  
  /* Read the status register */
  valid_flags = RIIC1.ICSR2.BYTE;
  /* Remove any that are not enabled */
  valid_flags &= RIIC1.ICIER.BYTE;
  /* Remove the transmit and receive flags */
  valid_flags &= 0x1Fu;

  /* Start detected? */
  if ((valid_flags & BIT_2) != 0x0u)
  {
    /* Disable Start interrupt request generation */
    RIIC1.ICIER.BIT.STIE = 0;
    /* Decide what to send */
    switch(rpdl_IIC_next_state[1])
    {
      case MASTER_SEND_SLAVE_ADDRESS_7:
        /* Send the slave address */
        RIIC1.ICDRT = rpdl_IIC_slave_address_lower[1];

        /* Transmit mode? */
        if ((rpdl_IIC_slave_address_lower[1] & BIT_0) == 0)
        {
          rpdl_IIC_current_state[1] = MASTER_SEND_DATA;
        }
        else
        {
          rpdl_IIC_current_state[1] = MASTER_START_READ;
        }
        break;
      case MASTER_SEND_SLAVE_ADDRESS_10a:
        rpdl_IIC_current_state[1] = MASTER_SEND_SLAVE_ADDRESS_10b;

        /* Send the first part of the slave address */
        RIIC1.ICDRT = rpdl_IIC_slave_address_upper[1];
        break;
      default:
        break;
    }
  }
  /* NACK detected? */
  else if ((valid_flags & BIT_4) != 0x0u)
  {
    /* Disable NACK interrupt request generation */
    RIIC1.ICIER.BIT.NAKIE = 0;

    /* Failed transmission of data? */
    if (rpdl_IIC_current_state[1] == MASTER_SEND_DATA)
    {
      /* Decrement the counter */
      rpdl_IIC_tx_counter[1]--;

      /* Call the callback function */
      if (rpdl_IIC_callback_func[1] != PDL_NO_FUNC)
      {
        rpdl_IIC_callback_func[1]();
      }
    }
    /* NACK received from the master? */
    else if (rpdl_IIC_current_state[1] == SLAVE_SEND_DATA)
    {
      /* Do a dummy read to release SCL */
      byte = RIIC1.ICDRR;
    }
  }
  else
  {
    /* Disable all interrupt request generation */
    RIIC1.ICIER.BYTE = 0x00u;
      
    /* Call the callback function */
    if (rpdl_IIC_callback_func[1] != PDL_NO_FUNC)
    {
      rpdl_IIC_callback_func[1]();
    }
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: IICn receive data interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_IIC_ICRXIn(void)
*-------------------------------------------------------------------
* Function    : 
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Output    : ICDRR, ICIER
*-------------------------------------------------------------------
* Use function  : 
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2010.01.25
*        : Ver 0.10
*""FUNC COMMENT END""**********************************************/
#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ICRXI0
__fast_interrupt void Interrupt_IIC_ICRXI0(void)
#else
#pragma vector = PDL_INTC_VECTOR_ICRXI0
__interrupt void Interrupt_IIC_ICRXI0(void)
#endif
{
  volatile uint8_t byte;
  switch (rpdl_IIC_current_state[0])
  {
    case MASTER_START_READ:
      /* Only one byte to be read? */
      if (rpdl_IIC_rx_threshold[0] == 1)
      {
        /* Prepare to signal a NACK to the slave (ACKBT = 1) */
        RIIC0.ICMR3.BIT.ACKBT = 1;
      }
      rpdl_IIC_current_state[0] = MASTER_READ_DATA;

      /* Do a dummy read */
      byte = RIIC0.ICDRR;
      break;
    case MASTER_READ_DATA:
      /* Is the last byte about to be read from the slave? */
      if (rpdl_IIC_rx_counter[0] == (rpdl_IIC_rx_threshold[0] - 2))
      {
        /* Prepare to signal a NACK to the slave (ACKBT = 1) */
        RIIC0.ICMR3.BIT.ACKBT = 1;
      }
      /* All data read from the slave? */
      else if (rpdl_IIC_rx_counter[0] == (rpdl_IIC_rx_threshold[0] - 1))
      {
        /* Enable Stop detection */
        RIIC0.ICIER.BIT.SPIE = 1;

        /* Issue a stop condition */
        RIIC0.ICCR2.BIT.SP = 1;
      }

      /* Store the data byte */
      *rpdl_IIC_rx_data_pointer[0]++ = RIIC0.ICDRR;

      /* Increment the counter */
      rpdl_IIC_rx_counter[0]++;
      break;
    case SLAVE_MONITOR:
      /* Do a dummy read */
      byte = RIIC0.ICDRR;

      /* Call the callback function */
      if (rpdl_IIC_callback_func[0] != PDL_NO_FUNC)
      {
        rpdl_IIC_callback_func[0]();
      }
      break;
    case SLAVE_READ_DATA:
      /* All data received? */
      if (rpdl_IIC_rx_counter[0] == rpdl_IIC_rx_threshold[0])
      {
        /* Do a dummy read */
        byte = RIIC0.ICDRR;

        /* Signal a NACK to the master */
        RIIC0.ICMR3.BIT.ACKBT = 1;
      }
      else
      {
        /* Store the data byte */
        *rpdl_IIC_rx_data_pointer[0]++ = RIIC0.ICDRR;

        /* Increment the counter */
        rpdl_IIC_rx_counter[0]++;

        /* Signal an ACK to the master */
        RIIC0.ICMR3.BIT.ACKBT = 0;
      }
      break;
    default:
      break;
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ICRXI1
__fast_interrupt void Interrupt_IIC_ICRXI1(void)
#else
#pragma vector = PDL_INTC_VECTOR_ICRXI1
__interrupt void Interrupt_IIC_ICRXI1(void)
#endif
{
  volatile uint8_t byte;
  switch (rpdl_IIC_current_state[1])
  {
    case MASTER_START_READ:
      /* Only one byte to be read? */
      if (rpdl_IIC_rx_threshold[1] == 1)
      {
        /* Prepare to signal a NACK to the slave (ACKBT = 1) */
        RIIC1.ICMR3.BIT.ACKBT = 1;
      }
      rpdl_IIC_current_state[1] = MASTER_READ_DATA;

      /* Do a dummy read */
      byte = RIIC1.ICDRR;
      break;
    case MASTER_READ_DATA:
      /* Is the last byte about to be read from the slave? */
      if (rpdl_IIC_rx_counter[1] == (rpdl_IIC_rx_threshold[1] - 2))
      {
        /* Prepare to signal a NACK to the slave (ACKBT = 1) */
        RIIC1.ICMR3.BIT.ACKBT = 1;
      }
      /* All data read from the slave? */
      else if (rpdl_IIC_rx_counter[1] == (rpdl_IIC_rx_threshold[1] - 1))
      {
        /* Enable Stop detection */
        RIIC1.ICIER.BIT.SPIE = 1;

        /* Issue a stop condition */
        RIIC1.ICCR2.BIT.SP = 1;
      }

      /* Store the data byte */
      *rpdl_IIC_rx_data_pointer[1]++ = RIIC1.ICDRR;

      /* Increment the counter */
      rpdl_IIC_rx_counter[1]++;
      break;
    case SLAVE_MONITOR:
      /* Do a dummy read */
      byte = RIIC1.ICDRR;

      /* Call the callback function */
      if (rpdl_IIC_callback_func[1] != PDL_NO_FUNC)
      {
        rpdl_IIC_callback_func[1]();
      }
      break;
    case SLAVE_READ_DATA:
      /* All data received? */
      if (rpdl_IIC_rx_counter[1] == rpdl_IIC_rx_threshold[1])
      {
        /* Do a dummy read */
        byte = RIIC1.ICDRR;

        /* Signal a NACK to the master */
        RIIC1.ICMR3.BIT.ACKBT = 1;
      }
      else
      {
        /* Store the data byte */
        *rpdl_IIC_rx_data_pointer[1]++ = RIIC1.ICDRR;

        /* Increment the counter */
        rpdl_IIC_rx_counter[1]++;

        /* Signal an ACK to the master */
        RIIC1.ICMR3.BIT.ACKBT = 0;
      }
      break;
    default:
      break;
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: IICn transmit data interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_IIC_ICTXIn(void)
*-------------------------------------------------------------------
* Function    : 
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Output    : ICDRT, ICIER
*-------------------------------------------------------------------
* Use function  : 
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2010.02.25
*        : Ver 0.10
*""FUNC COMMENT END""**********************************************/
#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ICTXI0
__fast_interrupt void Interrupt_IIC_ICTXI0(void)
#else
#pragma vector = PDL_INTC_VECTOR_ICTXI0
__interrupt void Interrupt_IIC_ICTXI0(void)
#endif
{
  switch (rpdl_IIC_current_state[0])
  {
    case MASTER_SEND_SLAVE_ADDRESS_10b:
      /* Send the second part of the slave address */
      RIIC0.ICDRT = rpdl_IIC_slave_address_lower[0];

      /* Transmit mode? */
      if ((rpdl_IIC_slave_address_upper[0] & BIT_0) == 0)
      {
        rpdl_IIC_current_state[0] = MASTER_SEND_DATA;
      }
      else
      {
        rpdl_IIC_current_state[0] = MASTER_START_READ;

        /* Enable receive interrupt request generation */
        RIIC0.ICIER.BIT.RIE = 1;
      }
      break;
    case MASTER_SEND_DATA:
      /* All data sent? */
      if (rpdl_IIC_tx_counter[0] == rpdl_IIC_tx_threshold[0])
      {
        rpdl_IIC_current_state[0] = MASTER_WAIT_FOR_TX_COMPLETE;

        /* Disable ICTXI interrupt request generation */
        RIIC0.ICIER.BIT.TIE = 0;

        /* Ensure the ICTEI IR flag is clear */
        ICU.IR[PDL_INTC_VECTOR_ICTEI0].BIT.IR = 0;

        /* Enable Transmit End interrupt generation */
        RIIC0.ICIER.BIT.TEIE = 1;
      }
      else
      {
        /* Load the data to be sent */
        RIIC0.ICDRT = *rpdl_IIC_tx_data_pointer[0]++;

        /* Increment the counter */
        rpdl_IIC_tx_counter[0]++;
      }
      break;
    case SLAVE_MONITOR:
      /* Call the callback function */
      if (rpdl_IIC_callback_func[0] != PDL_NO_FUNC)
      {
        rpdl_IIC_callback_func[0]();
      }
      break;
    case SLAVE_SEND_DATA:
      /* All data sent? */
      if (rpdl_IIC_tx_counter[0] == rpdl_IIC_tx_threshold[0])
      {
        /* Loop back to the start */
        rpdl_IIC_tx_data_pointer[0] -= rpdl_IIC_tx_threshold[0];
      }

      /* Load the data to be sent */
      RIIC0.ICDRT = *rpdl_IIC_tx_data_pointer[0]++;

      /* Increment the counter */
      rpdl_IIC_tx_counter[0]++;
      break;
    default:
      break;
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ICTXI1
__fast_interrupt void Interrupt_IIC_ICTXI1(void)
#else
#pragma vector = PDL_INTC_VECTOR_ICTXI1
__interrupt void Interrupt_IIC_ICTXI1(void)
#endif
{
  switch (rpdl_IIC_current_state[1])
  {
    case MASTER_SEND_SLAVE_ADDRESS_10b:
      /* Send the second part of the slave address */
      RIIC1.ICDRT = rpdl_IIC_slave_address_lower[1];

      /* Transmit mode? */
      if ((rpdl_IIC_slave_address_upper[1] & BIT_0) == 0)
      {
        rpdl_IIC_current_state[1] = MASTER_SEND_DATA;
      }
      else
      {
        rpdl_IIC_current_state[1] = MASTER_START_READ;

        /* Enable receive interrupt request generation */
        RIIC1.ICIER.BIT.RIE = 1;
      }
      break;
    case MASTER_SEND_DATA:
      /* All data sent? */
      if (rpdl_IIC_tx_counter[1] == rpdl_IIC_tx_threshold[1])
      {
        rpdl_IIC_current_state[1] = MASTER_WAIT_FOR_TX_COMPLETE;

        /* Disable ICTXI interrupt request generation */
        RIIC1.ICIER.BIT.TIE = 0;

        /* Ensure the ICTEI IR flag is clear */
        ICU.IR[PDL_INTC_VECTOR_ICTEI1].BIT.IR = 0;

        /* Enable Transmit End interrupt generation */
        RIIC1.ICIER.BIT.TEIE = 1;
      }
      else
      {
        /* Load the data to be sent */
        RIIC1.ICDRT = *rpdl_IIC_tx_data_pointer[1]++;

        /* Increment the counter */
        rpdl_IIC_tx_counter[1]++;
      }
      break;
    case SLAVE_MONITOR:
      /* Call the callback function */
      if (rpdl_IIC_callback_func[1] != PDL_NO_FUNC)
      {
        rpdl_IIC_callback_func[1]();
      }
      break;
    case SLAVE_SEND_DATA:
      /* All data sent? */
      if (rpdl_IIC_tx_counter[1] == rpdl_IIC_tx_threshold[1])
      {
        /* Loop back to the start */
        rpdl_IIC_tx_data_pointer[1] -= rpdl_IIC_tx_threshold[1];
      }

      /* Load the data to be sent */
      RIIC1.ICDRT = *rpdl_IIC_tx_data_pointer[1]++;

      /* Increment the counter */
      rpdl_IIC_tx_counter[1]++;
      break;
    default:
      break;
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: IICn transmit end interrupt processing
*-------------------------------------------------------------------
* Declaration  : void InterruptIIC_ICTEIn(void)
*-------------------------------------------------------------------
* Function    : 
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Output    : ICIER, ICSR2
*-------------------------------------------------------------------
* Use function  : 
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2010.01.18
*        : Ver 0.10
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ICTEI0
__fast_interrupt void Interrupt_IIC_ICTEI0(void)
#else
#pragma vector = PDL_INTC_VECTOR_ICTEI0
__interrupt void Interrupt_IIC_ICTEI0(void)
#endif
{
  /* Disable Transmit End interrupt request generation */
  RIIC0.ICIER.BIT.TEIE = 0;
  /* Clear the flag */
  RIIC0.ICSR2.BIT.TEND = 0;

  /* Stop condition required? */
  if (rpdl_IIC_stop[0] == true)
  {
    rpdl_IIC_current_state[0] = MASTER_WAIT_FOR_STOP;

    /* Enable Stop detection */
    RIIC0.ICIER.BIT.SPIE = 1;

    /* Issue a stop condition */
    RIIC0.ICCR2.BIT.SP = 1;
  }
  else
  {
    /* Disable all interrupt request generation */
    RIIC0.ICIER.BYTE = 0x00u;

    /* Call the callback function */
    if (rpdl_IIC_callback_func[0] != PDL_NO_FUNC)
    {
      rpdl_IIC_callback_func[0]();
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ICTEI1
__fast_interrupt void Interrupt_IIC_ICTEI1(void)
#else
#pragma vector = PDL_INTC_VECTOR_ICTEI1
__interrupt void Interrupt_IIC_ICTEI1(void)
#endif
{
  /* Disable Transmit End interrupt request generation */
  RIIC1.ICIER.BIT.TEIE = 0;
  /* Clear the flag */
  RIIC1.ICSR2.BIT.TEND = 0;

  /* Stop condition required? */
  if (rpdl_IIC_stop[1] == true)
  {
    rpdl_IIC_current_state[1] = MASTER_WAIT_FOR_STOP;

    /* Enable Stop detection */
    RIIC1.ICIER.BIT.SPIE = 1;

    /* Issue a stop condition */
    RIIC1.ICCR2.BIT.SP = 1;
  }
  else
  {
    /* Disable all interrupt request generation */
    RIIC1.ICIER.BYTE = 0x00u;

    /* Call the callback function */
    if (rpdl_IIC_callback_func[1] != PDL_NO_FUNC)
    {
      rpdl_IIC_callback_func[1]();
    }
  }
}
/* End of file */
