/* Adapted for use with IAR Embedded Workbench */
/*""FILE COMMENT""*******************************************************
* System Name  : Interrupt program for RX610xV
* File Name    : Interrupt_SCI.c
* Version    : 0.10
* Contents    : Interrupt handlers for all SCI channels
* Customer    : 
* Model      : 
* Order      : 
* CPU      : RX600
* Compiler    : RXC
* OS      : Nothing
* Programmer  : 
* Note      : 
************************************************************************
* Copyright,2009 RENESAS TECHNOLOGY CORPORATION
* AND RENESAS SOLUTIONS CORPORATION
************************************************************************
* History    : 2009.09.08
*        : Ver 0.10
*""FILE COMMENT END""**************************************************/

#include "r_pdl_sci.h"
#include "r_pdl_definitions.h"

/*""FUNC COMMENT""***************************************************
* Module outline: SCIn receive data error interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_SCI0_ERIn(void)
*-------------------------------------------------------------------
* Function    : 
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Output    : SSR for that SCI channel
*-------------------------------------------------------------------
* Use function  : rpdl_SCI_RX_Error_callback_func
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2009.09.25
*        : Ver 0.10
*        : 2009.11.26
*        : Ver 0.11
*        : Removed the Error callback function type
*        : 2010.02.15
*        : Ver 0.12
*        : Removed the Break callback function
*        : 2010.02.22
*        : Ver 0.13
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ERI0
__fast_interrupt void Interrupt_SCI0_ERI0(void)
#else
#pragma vector = PDL_INTC_VECTOR_ERI0
__interrupt void Interrupt_SCI0_ERI0(void)
#endif
{
  /* Will the user handle the errors? */
  if (rpdl_SCI_RX_Error_callback_func[0] != PDL_NO_FUNC)
  {
    /* Call the error handler */
    rpdl_SCI_RX_Error_callback_func[0]();
  }
  else
  {
    /* Clear the error flags */
    SCI0.SSR.BYTE = (BIT_7 | BIT_6);
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ERI1
__fast_interrupt void Interrupt_SCI1_ERI1(void)
#else
#pragma vector = PDL_INTC_VECTOR_ERI1
__interrupt void Interrupt_SCI1_ERI1(void)
#endif
{
  /* Will the user handle the errors? */
  if (rpdl_SCI_RX_Error_callback_func[1] != PDL_NO_FUNC)
  {
    /* Call the error handler */
    rpdl_SCI_RX_Error_callback_func[1]();
  }
  else
  {
    /* Clear the error flags */
    SCI1.SSR.BYTE = (BIT_7 | BIT_6);
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ERI2
__fast_interrupt void Interrupt_SCI2_ERI2(void)
#else
#pragma vector = PDL_INTC_VECTOR_ERI2
__interrupt void Interrupt_SCI2_ERI2(void)
#endif
{
  /* Will the user handle the errors? */
  if (rpdl_SCI_RX_Error_callback_func[2] != PDL_NO_FUNC)
  {
    /* Call the error handler */
    rpdl_SCI_RX_Error_callback_func[2]();
  }
  else
  {
    /* Clear the error flags */
    SCI2.SSR.BYTE = (BIT_7 | BIT_6);
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ERI3
__fast_interrupt void Interrupt_SCI3_ERI3(void)
#else
#pragma vector = PDL_INTC_VECTOR_ERI3
__interrupt void Interrupt_SCI3_ERI3(void)
#endif
{
  /* Will the user handle the errors? */
  if (rpdl_SCI_RX_Error_callback_func[3] != PDL_NO_FUNC)
  {
    /* Call the error handler */
    rpdl_SCI_RX_Error_callback_func[3]();
  }
  else
  {
    /* Clear the error flags */
    SCI3.SSR.BYTE = (BIT_7 | BIT_6);
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ERI4
__fast_interrupt void Interrupt_SCI4_ERI4(void)
#else
#pragma vector = PDL_INTC_VECTOR_ERI4
__interrupt void Interrupt_SCI4_ERI4(void)
#endif
{
  /* Will the user handle the errors? */
  if (rpdl_SCI_RX_Error_callback_func[4] != PDL_NO_FUNC)
  {
    /* Call the error handler */
    rpdl_SCI_RX_Error_callback_func[4]();
  }
  else
  {
    /* Clear the error flags */
    SCI4.SSR.BYTE = (BIT_7 | BIT_6);
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ERI5
__fast_interrupt void Interrupt_SCI5_ERI5(void)
#else
#pragma vector = PDL_INTC_VECTOR_ERI5
__interrupt void Interrupt_SCI5_ERI5(void)
#endif
{
  /* Will the user handle the errors? */
  if (rpdl_SCI_RX_Error_callback_func[5] != PDL_NO_FUNC)
  {
    /* Call the error handler */
    rpdl_SCI_RX_Error_callback_func[5]();
  }
  else
  {
    /* Clear the error flags */
    SCI5.SSR.BYTE = (BIT_7 | BIT_6);
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_ERI6
__fast_interrupt void Interrupt_SCI6_ERI6(void)
#else
#pragma vector = PDL_INTC_VECTOR_ERI6
__interrupt void Interrupt_SCI6_ERI6(void)
#endif
{
  /* Will the user handle the errors? */
  if (rpdl_SCI_RX_Error_callback_func[6] != PDL_NO_FUNC)
  {
    /* Call the error handler */
    rpdl_SCI_RX_Error_callback_func[6]();
  }
  else
  {
    /* Clear the error flags */
    SCI6.SSR.BYTE = (BIT_7 | BIT_6);
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: SCIn receive data interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_SCIn_RXIn(void)
*-------------------------------------------------------------------
* Function    : 
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Output    : 
*-------------------------------------------------------------------
* Use function  : rpdl_SCI_RX_End_callback_func
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2009.10.02
*        : Ver 0.10
*        : 2009.11.26
*        : Ver 0.11
*        : Modified DMAC control
*        : 2010.02.02
*        : Ver 0.12
*        : Added callback function checking
*        : 2010.02.08
*        : Ver 0.13
*        : Removed DMAC trigger control
*        : 2010.02.12
*        : Ver 0.14
*        : Added reception disabling once all data is received.
*        : 2010.02.22
*        : Ver 0.15
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_RXI0
__fast_interrupt void Interrupt_SCI0_RXI0(void)
#else
#pragma vector = PDL_INTC_VECTOR_RXI0
__interrupt void Interrupt_SCI0_RXI0(void)
#endif
{
  /* Ok to process the data? */
  if (rpdl_SCI_rx_using_irq[0] == true)
  {
    /* Read and store the character */
    *rpdl_SCI_rx_string_pointer[0] = SCI0.RDR;

    /* Increment the character counter */
    rpdl_SCI_rx_counter[0]++;

    /* More characters expected? */
    if (rpdl_SCI_rx_counter[0] < rpdl_SCI_rx_threshold[0])
    {
      /* Move to the next location in the buffer */
      rpdl_SCI_rx_string_pointer[0]++;
    }
    else
    {
      /* Disable interrupt request generation, and try to disable reception */
      SCI0.SCR.BYTE &= (INV_BIT_6 & INV_BIT_4);

      /* Notify the user */
      if (rpdl_SCI_RX_End_callback_func[0] != PDL_NO_FUNC)
      {
        rpdl_SCI_RX_End_callback_func[0]();
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_RXI1
__fast_interrupt void Interrupt_SCI1_RXI1(void)
#else
#pragma vector = PDL_INTC_VECTOR_RXI1
__interrupt void Interrupt_SCI1_RXI1(void)
#endif
{
  /* Ok to process the data? */
  if (rpdl_SCI_rx_using_irq[1] == true)
  {
    /* Read and store the character */
    *rpdl_SCI_rx_string_pointer[1] = SCI1.RDR;
    /* Increment the character counter */
    rpdl_SCI_rx_counter[1]++;

    /* More characters expected? */
    if (rpdl_SCI_rx_counter[1] < rpdl_SCI_rx_threshold[1])
    {
      /* Move to the next location in the buffer */
      rpdl_SCI_rx_string_pointer[1]++;
    }
    else
    {
      /* Disable interrupt request generation, and try to disable reception */
      SCI1.SCR.BYTE &= (INV_BIT_6 & INV_BIT_4);

      /* Notify the user */
      if (rpdl_SCI_RX_End_callback_func[1] != PDL_NO_FUNC)
      {
        rpdl_SCI_RX_End_callback_func[1]();
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_RXI2
__fast_interrupt void Interrupt_SCI2_RXI2(void)
#else
#pragma vector = PDL_INTC_VECTOR_RXI2
__interrupt void Interrupt_SCI2_RXI2(void)
#endif
{
  /* Ok to process the data? */
  if (rpdl_SCI_rx_using_irq[2] == true)
  {
    /* Read and store the character */
    *rpdl_SCI_rx_string_pointer[2] = SCI2.RDR;

    /* Increment the character counter */
    rpdl_SCI_rx_counter[2]++;

    /* More characters expected? */
    if (rpdl_SCI_rx_counter[2] < rpdl_SCI_rx_threshold[2])
    {
      /* Move to the next location in the buffer */
      rpdl_SCI_rx_string_pointer[2]++;
    }
    else
    {
      /* Disable interrupt request generation, and try to disable reception */
      SCI2.SCR.BYTE &= (INV_BIT_6 & INV_BIT_4);

      /* Notify the user */
      if (rpdl_SCI_RX_End_callback_func[2] != PDL_NO_FUNC)
      {
        rpdl_SCI_RX_End_callback_func[2]();
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_RXI3
__fast_interrupt void Interrupt_SCI3_RXI3(void)
#else
#pragma vector = PDL_INTC_VECTOR_RXI3
__interrupt void Interrupt_SCI3_RXI3(void)
#endif
{
  /* Ok to process the data? */
  if (rpdl_SCI_rx_using_irq[3] == true)
  {
    /* Read and store the character */
    *rpdl_SCI_rx_string_pointer[3] = SCI3.RDR;

    /* Increment the character counter */
    rpdl_SCI_rx_counter[3]++;

    /* More characters expected? */
    if (rpdl_SCI_rx_counter[3] < rpdl_SCI_rx_threshold[3])
    {
      /* Move to the next location in the buffer */
      rpdl_SCI_rx_string_pointer[3]++;
    }
    else
    {
      /* Disable interrupt request generation, and try to disable reception */
      SCI3.SCR.BYTE &= (INV_BIT_6 & INV_BIT_4);

      /* Notify the user */
      if (rpdl_SCI_RX_End_callback_func[3] != PDL_NO_FUNC)
      {
        rpdl_SCI_RX_End_callback_func[3]();
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_RXI4
__fast_interrupt void Interrupt_SCI4_RXI4(void)
#else
#pragma vector = PDL_INTC_VECTOR_RXI4
__interrupt void Interrupt_SCI4_RXI4(void)
#endif
{
  /* Ok to process the data? */
  if (rpdl_SCI_rx_using_irq[4] == true)
  {
    /* Read and store the character */
    *rpdl_SCI_rx_string_pointer[4] = SCI4.RDR;

    /* Increment the character counter */
    rpdl_SCI_rx_counter[4]++;

    /* More characters expected? */
    if (rpdl_SCI_rx_counter[4] < rpdl_SCI_rx_threshold[4])
    {
      /* Move to the next location in the buffer */
      rpdl_SCI_rx_string_pointer[4]++;
    }
    else
    {
      /* Disable interrupt request generation, and try to disable reception */
      SCI4.SCR.BYTE &= (INV_BIT_6 & INV_BIT_4);

      /* Notify the user */
      if (rpdl_SCI_RX_End_callback_func[4] != PDL_NO_FUNC)
      {
        rpdl_SCI_RX_End_callback_func[4]();
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_RXI5
__fast_interrupt void Interrupt_SCI5_RXI5(void)
#else
#pragma vector = PDL_INTC_VECTOR_RXI5
__interrupt void Interrupt_SCI5_RXI5(void)
#endif
{
  /* Ok to process the data? */
  if (rpdl_SCI_rx_using_irq[5] == true)
  {
    /* Read and store the character */
    *rpdl_SCI_rx_string_pointer[5] = SCI5.RDR;

    /* Increment the character counter */
    rpdl_SCI_rx_counter[5]++;

    /* More characters expected? */
    if (rpdl_SCI_rx_counter[5] < rpdl_SCI_rx_threshold[5])
    {
      /* Move to the next location in the buffer */
      rpdl_SCI_rx_string_pointer[5]++;
    }
    else
    {
      /* Disable interrupt request generation, and try to disable reception */
      SCI5.SCR.BYTE &= (INV_BIT_6 & INV_BIT_4);

      /* Notify the user */
      if (rpdl_SCI_RX_End_callback_func[5] != PDL_NO_FUNC)
      {
        rpdl_SCI_RX_End_callback_func[5]();
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_RXI6
__fast_interrupt void Interrupt_SCI6_RXI6(void)
#else
#pragma vector = PDL_INTC_VECTOR_RXI6
__interrupt void Interrupt_SCI6_RXI6(void)
#endif
{
  /* Ok to process the data? */
  if (rpdl_SCI_rx_using_irq[6] == true)
  {
    /* Read and store the character */
    *rpdl_SCI_rx_string_pointer[6] = SCI6.RDR;

    /* Increment the character counter */
    rpdl_SCI_rx_counter[6]++;

    /* More characters expected? */
    if (rpdl_SCI_rx_counter[6] < rpdl_SCI_rx_threshold[6])
    {
      /* Move to the next location in the buffer */
      rpdl_SCI_rx_string_pointer[6]++;
    }
    else
    {
      /* Disable interrupt request generation, and try to disable reception */
      SCI6.SCR.BYTE &= (INV_BIT_6 & INV_BIT_4);

      /* Notify the user */
      if (rpdl_SCI_RX_End_callback_func[6] != PDL_NO_FUNC)
      {
        rpdl_SCI_RX_End_callback_func[6]();
      }
    }
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: SCIn transmit data interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_SCIn_TXIn(void)
*-------------------------------------------------------------------
* Function    : 
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Output    : TDR for that SCI channel
*-------------------------------------------------------------------
* Use function  : 
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2009.09.25
*        : Ver 0.10
*        : 2009.10.12
*        : Ver 0.11
*        : Added IR and SCR control
*        : Added DMAC control
*        : 2009.11.26
*        : Ver 0.12
*        : Modified DMAC control
*        : 2010.02.08
*        : Ver 0.13
*        : Removed DMAC trigger control
*        : 2010.02.22
*        : Ver 0.14
*        : Removed fast interrupt code
*        : Added Smart card mode check
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TXI0
__fast_interrupt void Interrupt_SCI0_TXI0(void)
#else
#pragma vector = PDL_INTC_VECTOR_TXI0
__interrupt void Interrupt_SCI0_TXI0(void)
#endif
{
  /* Ok to process the string? */
  if (rpdl_SCI_tx_using_irq[0] == true)
  {
    /* Another character to be sent? */
    if (
    /* Sending a string and next character is not a Null? */
    ((rpdl_SCI_tx_threshold[0u] == 0) && (*rpdl_SCI_tx_string_pointer[0] != NULL))
    ||
    /* Not reached the byte total? */
    (rpdl_SCI_tx_counter[0] < rpdl_SCI_tx_threshold[0])
    )
    {
      /* Send the character */
      SCI0.TDR = *rpdl_SCI_tx_string_pointer[0]++;

      /* Increment the counter */
      rpdl_SCI_tx_counter[0]++;
    }
    else
    {
      /* Disable interrupt request generation */
      SCI0.SCR.BIT.TIE = 0;

      /* Smart card mode? */
      if (SCI0.SCMR.BIT.SMIF == 1)
      {
        /* Disable transmission and interrupt request generation */
        SCI0.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5);

        /* Call the callback function */
        if (rpdl_SCI_TX_End_callback_func[0] != PDL_NO_FUNC)
        {
          rpdl_SCI_TX_End_callback_func[0]();
        }
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TXI1
__fast_interrupt void Interrupt_SCI1_TXI1(void)
#else
#pragma vector = PDL_INTC_VECTOR_TXI1
__interrupt void Interrupt_SCI1_TXI1(void)
#endif
{
  /* Ok to process the string? */
  if (rpdl_SCI_tx_using_irq[1] == true)
  {
    /* Another character to be sent? */
    if (
    /* Sending a string and next character is not a Null? */
    ((rpdl_SCI_tx_threshold[1] == 0) && (*rpdl_SCI_tx_string_pointer[1] != NULL))
    ||
    /* Not reached the byte total? */
    (rpdl_SCI_tx_counter[1] < rpdl_SCI_tx_threshold[1])
    )
    {
      /* Send the character */
      SCI1.TDR = *rpdl_SCI_tx_string_pointer[1]++;

      /* Increment the counter */
      rpdl_SCI_tx_counter[1]++;
    }
    else
    {
      /* Disable interrupt request generation */
      SCI1.SCR.BIT.TIE = 0;

      /* Smart card mode? */
      if (SCI1.SCMR.BIT.SMIF == 1)
      {
        /* Disable transmission and interrupt request generation */
        SCI1.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5);

        /* Call the callback function */
        if (rpdl_SCI_TX_End_callback_func[1] != PDL_NO_FUNC)
        {
          rpdl_SCI_TX_End_callback_func[1]();
        }
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TXI2
__fast_interrupt void Interrupt_SCI2_TXI2(void)
#else
#pragma vector = PDL_INTC_VECTOR_TXI2
__interrupt void Interrupt_SCI2_TXI2(void)
#endif
{
  /* Ok to process the string? */
  if (rpdl_SCI_tx_using_irq[2] == true)
  {
    /* Another character to be sent? */
    if (
    /* Sending a string and next character is not a Null? */
    ((rpdl_SCI_tx_threshold[2] == 0) && (*rpdl_SCI_tx_string_pointer[2] != NULL))
    ||
    /* Not reached the byte total? */
    (rpdl_SCI_tx_counter[2] < rpdl_SCI_tx_threshold[2])
    )
    {
      /* Send the character */
      SCI2.TDR = *rpdl_SCI_tx_string_pointer[2]++;

      /* Increment the counter */
      rpdl_SCI_tx_counter[2]++;
    }
    else
    {
      /* Disable interrupt request generation */
      SCI2.SCR.BIT.TIE = 0;

      /* Smart card mode? */
      if (SCI2.SCMR.BIT.SMIF == 1)
      {
        /* Disable transmission and interrupt request generation */
        SCI2.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5);

        /* Call the callback function */
        if (rpdl_SCI_TX_End_callback_func[2] != PDL_NO_FUNC)
        {
          rpdl_SCI_TX_End_callback_func[2]();
        }
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TXI3
__fast_interrupt void Interrupt_SCI3_TXI3(void)
#else
#pragma vector = PDL_INTC_VECTOR_TXI3
__interrupt void Interrupt_SCI3_TXI3(void)
#endif
{
  /* Ok to process the string? */
  if (rpdl_SCI_tx_using_irq[3] == true)
  {
    /* Another character to be sent? */
    if (
    /* Sending a string and next character is not a Null? */
    ((rpdl_SCI_tx_threshold[3] == 0) && (*rpdl_SCI_tx_string_pointer[3] != NULL))
    ||
    /* Not reached the byte total? */
    (rpdl_SCI_tx_counter[3] < rpdl_SCI_tx_threshold[3])
    )
    {
      /* Send the character */
      SCI3.TDR = *rpdl_SCI_tx_string_pointer[3]++;

      /* Increment the counter */
      rpdl_SCI_tx_counter[3]++;
    }
    else
    {
      /* Disable interrupt request generation */
      SCI3.SCR.BIT.TIE = 0;

      /* Smart card mode? */
      if (SCI3.SCMR.BIT.SMIF == 1)
      {
        /* Disable transmission and interrupt request generation */
        SCI3.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5);

        /* Call the callback function */
        if (rpdl_SCI_TX_End_callback_func[3] != PDL_NO_FUNC)
        {
          rpdl_SCI_TX_End_callback_func[3]();
        }
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TXI4
__fast_interrupt void Interrupt_SCI4_TXI4(void)
#else
#pragma vector = PDL_INTC_VECTOR_TXI4
__interrupt void Interrupt_SCI4_TXI4(void)
#endif
{
  /* Ok to process the string? */
  if (rpdl_SCI_tx_using_irq[4] == true)
  {
    /* Another character to be sent? */
    if (
    /* Sending a string and next character is not a Null? */
    ((rpdl_SCI_tx_threshold[4] == 0) && (*rpdl_SCI_tx_string_pointer[4] != NULL))
    ||
    /* Not reached the byte total? */
    (rpdl_SCI_tx_counter[4] < rpdl_SCI_tx_threshold[4])
    )
    {
      /* Send the character */
      SCI4.TDR = *rpdl_SCI_tx_string_pointer[4]++;

      /* Increment the counter */
      rpdl_SCI_tx_counter[4]++;
    }
    else
    {
      /* Disable interrupt request generation */
      SCI4.SCR.BIT.TIE = 0;

      /* Smart card mode? */
      if (SCI4.SCMR.BIT.SMIF == 1)
      {
        /* Disable transmission and interrupt request generation */
        SCI4.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5);

        /* Call the callback function */
        if (rpdl_SCI_TX_End_callback_func[4] != PDL_NO_FUNC)
        {
          rpdl_SCI_TX_End_callback_func[4]();
        }
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TXI5
__fast_interrupt void Interrupt_SCI5_TXI5(void)
#else
#pragma vector = PDL_INTC_VECTOR_TXI5
__interrupt void Interrupt_SCI5_TXI5(void)
#endif
{
  /* Ok to process the string? */
  if (rpdl_SCI_tx_using_irq[5] == true)
  {
    /* Another character to be sent? */
    if (
    /* Sending a string and next character is not a Null? */
    ((rpdl_SCI_tx_threshold[5] == 0) && (*rpdl_SCI_tx_string_pointer[5] != NULL))
    ||
    /* Not reached the byte total? */
    (rpdl_SCI_tx_counter[5] < rpdl_SCI_tx_threshold[5])
    )
    {
      /* Send the character */
      SCI5.TDR = *rpdl_SCI_tx_string_pointer[5]++;

      /* Increment the counter */
      rpdl_SCI_tx_counter[5]++;
    }
    else
    {
      /* Disable interrupt request generation */
      SCI5.SCR.BIT.TIE = 0;

      /* Smart card mode? */
      if (SCI5.SCMR.BIT.SMIF == 1)
      {
        /* Disable transmission and interrupt request generation */
        SCI5.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5);

        /* Call the callback function */
        if (rpdl_SCI_TX_End_callback_func[5] != PDL_NO_FUNC)
        {
          rpdl_SCI_TX_End_callback_func[5]();
        }
      }
    }
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TXI6
__fast_interrupt void Interrupt_SCI6_TXI6(void)
#else
#pragma vector = PDL_INTC_VECTOR_TXI6
__interrupt void Interrupt_SCI6_TXI6(void)
#endif
{
  /* Ok to process the string? */
  if (rpdl_SCI_tx_using_irq[6] == true)
  {
    /* Another character to be sent? */
    if (
    /* Sending a string and next character is not a Null? */
    ((rpdl_SCI_tx_threshold[6] == 0) && (*rpdl_SCI_tx_string_pointer[6] != NULL))
    ||
    /* Not reached the byte total? */
    (rpdl_SCI_tx_counter[6] < rpdl_SCI_tx_threshold[6])
    )
    {
      /* Send the character */
      SCI6.TDR = *rpdl_SCI_tx_string_pointer[6]++;

      /* Increment the counter */
      rpdl_SCI_tx_counter[6]++;
    }
    else
    {
      /* Disable interrupt request generation */
      SCI6.SCR.BIT.TIE = 0;

      /* Smart card mode? */
      if (SCI6.SCMR.BIT.SMIF == 1)
      {
        /* Disable transmission and interrupt request generation */
        SCI6.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5);

        /* Call the callback function */
        if (rpdl_SCI_TX_End_callback_func[6] != PDL_NO_FUNC)
        {
          rpdl_SCI_TX_End_callback_func[6]();
        }
      }
    }
  }
}

/*""FUNC COMMENT""***************************************************
* Module outline: SCIn transmit end interrupt processing
*-------------------------------------------------------------------
* Declaration  : void Interrupt_SCIn_TEIn(void)
*-------------------------------------------------------------------
* Function    : 
*-------------------------------------------------------------------
* Argument    : Nothing
*-------------------------------------------------------------------
* Return value  : Nothing
*-------------------------------------------------------------------
* Output    : SCR for that channel
*-------------------------------------------------------------------
* Use function  : rpdl_SCI_TX_End_callback_func()
*-------------------------------------------------------------------
* Notes      : 
*-------------------------------------------------------------------
* History    : 2009.09.25
*        : Ver 0.10
*        : 2009.10.07
*        : Ver 0.11
*        : Changed the SCR control
*        : 2010.02.02
*        : Ver 0.12
*        : Added callback function checking
*        : 2010.02.15
*        : Ver 0.13
*        : Removed break output control
*        : 2010.02.22
*        : Ver 0.16
*        : Removed fast interrupt code
*""FUNC COMMENT END""**********************************************/

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TEI0
__fast_interrupt void Interrupt_SCI0_TEI0(void)
#else
#pragma vector = PDL_INTC_VECTOR_TEI0
__interrupt void Interrupt_SCI0_TEI0(void)
#endif
{
  /* Disable interrupt request generation, and try to disable transmission */
  SCI0.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5 & INV_BIT_2);

  /* Call the callback function */
  if (rpdl_SCI_TX_End_callback_func[0] != PDL_NO_FUNC)
  {
    rpdl_SCI_TX_End_callback_func[0]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TEI1
__fast_interrupt void Interrupt_SCI1_TEI1(void)
#else
#pragma vector = PDL_INTC_VECTOR_TEI1
__interrupt void Interrupt_SCI1_TEI1(void)
#endif
{
  /* Disable interrupt request generation, and try to disable transmission */
  SCI1.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5 & INV_BIT_2);

  /* Call the callback function */
  if (rpdl_SCI_TX_End_callback_func[1] != PDL_NO_FUNC)
  {
    rpdl_SCI_TX_End_callback_func[1]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TEI2
__fast_interrupt void Interrupt_SCI2_TEI2(void)
#else
#pragma vector = PDL_INTC_VECTOR_TEI2
__interrupt void Interrupt_SCI2_TEI2(void)
#endif
{
  /* Disable interrupt request generation, and try to disable transmission */
  SCI2.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5 & INV_BIT_2);

  /* Call the callback function */
  if (rpdl_SCI_TX_End_callback_func[2] != PDL_NO_FUNC)
  {
    rpdl_SCI_TX_End_callback_func[2]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TEI3
__fast_interrupt void Interrupt_SCI3_TEI3(void)
#else
#pragma vector = PDL_INTC_VECTOR_TEI3
__interrupt void Interrupt_SCI3_TEI3(void)
#endif
{
  /* Disable interrupt request generation, and try to disable transmission */
  SCI3.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5 & INV_BIT_2);

  /* Call the callback function */
  if (rpdl_SCI_TX_End_callback_func[3] != PDL_NO_FUNC)
  {
    rpdl_SCI_TX_End_callback_func[3]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TEI4
__fast_interrupt void Interrupt_SCI4_TEI4(void)
#else
#pragma vector = PDL_INTC_VECTOR_TEI4
__interrupt void Interrupt_SCI4_TEI4(void)
#endif
{
  /* Disable interrupt request generation, and try to disable transmission */
  SCI4.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5 & INV_BIT_2);

  /* Call the callback function */
  if (rpdl_SCI_TX_End_callback_func[4] != PDL_NO_FUNC)
  {
    rpdl_SCI_TX_End_callback_func[4]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TEI5
__fast_interrupt void Interrupt_SCI5_TEI5(void)
#else
#pragma vector = PDL_INTC_VECTOR_TEI5
__interrupt void Interrupt_SCI5_TEI5(void)
#endif
{
  /* Disable interrupt request generation, and try to disable transmission */
  SCI5.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5 & INV_BIT_2);

  /* Call the callback function */
  if (rpdl_SCI_TX_End_callback_func[5] != PDL_NO_FUNC)
  {
    rpdl_SCI_TX_End_callback_func[5]();
  }
}

#if FAST_INTC_VECTOR == PDL_INTC_VECTOR_TEI6
__fast_interrupt void Interrupt_SCI6_TEI6(void)
#else
#pragma vector = PDL_INTC_VECTOR_TEI6
__interrupt void Interrupt_SCI6_TEI6(void)
#endif
{
  /* Disable interrupt request generation, and try to disable transmission */
  SCI6.SCR.BYTE &= (INV_BIT_7 & INV_BIT_5 & INV_BIT_2);

  /* Call the callback function */
  if (rpdl_SCI_TX_End_callback_func[6] != PDL_NO_FUNC)
  {
    rpdl_SCI_TX_End_callback_func[6]();
  }
}
/* End of file */
