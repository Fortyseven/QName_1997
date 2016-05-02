/*
  -------------------------
  No, not THAT Q-Sound...:)
  -------------------------
*/


void voice_init()
{
}

void voice_done()
{
}

int voice_play(SAMPLE *sample)
{
  if (allocate_voice(sample) == -1)
    {
      return -1;  //Failure to communicate...
    }


}
