extern void notify_LED_time(void);		// for TRGA timer

// TGRA0								
#pragma vector=142
__interrupt void TGRA0_int(void)
{
  notify_LED_time();
}

