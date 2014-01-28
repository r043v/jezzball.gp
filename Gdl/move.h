/* this file is a part of Gdl 0.4f and was writen by rov, http://procvor.free.fr */

// not tested
char can_gfx_move(struct gfx * gfx, struct map * map,unsigned char powa,unsigned char bound_up,unsigned char bound_down,unsigned char bound_left,unsigned char bound_right, unsigned char masq, unsigned char sens)
{
	char cpt ;
	switch (sens)
	{
		case 'u':
			for(cpt=0;cpt<powa;cpt++)
			{
 				if(! test_tile(map,get_tile(map,gfx->pos_x + bound_left  - map->scrool_x, gfx->pos_y + bound_up - map->scrool_y),masq))
					if(! test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + bound_up - map->scrool_y),masq))
						return 1 ;
			} ;
 			break ;

		case 'd':
			for(cpt=0;cpt<powa;cpt++)
			{
 				if(! test_tile(map,get_tile(map,gfx->pos_x + bound_left  - map->scrool_x, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
					if(! test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
						return 1 ;
			} ;
 			break ;

		case 'l':
			for(cpt=0;cpt<powa;cpt++)
			{
 				if(! test_tile(map,get_tile(map,gfx->pos_x + bound_left  - map->scrool_x, gfx->pos_y + bound_up - map->scrool_y),masq))
					if(! test_tile(map,get_tile(map,gfx->pos_x + bound_left - map->scrool_x, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
						return 1 ;
			} ;
 			break ;

		case 'r':
			for(cpt=0;cpt<powa;cpt++)
			{
 				if(! test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + bound_up - map->scrool_y),masq))
					if(! test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
						return 1 ;
			} ;
 			break ;
	}

	return 0 ;
}


char reajust_sprite_on_map(struct gfx * gfx, struct map * map,unsigned char bound_up,unsigned char bound_down,unsigned char bound_left,unsigned char bound_right, unsigned char masq, unsigned char sens)
{
	switch (sens)
	{
		case 'u':
 				while(test_tile(map,get_tile(map,gfx->pos_x + bound_left - map->scrool_x, gfx->pos_y + bound_up + 1 - map->scrool_y),masq)||test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + bound_up + 1 - map->scrool_y),masq))
					gfx->pos_y++ ;
 			break ;

		case 'd':
 				while(test_tile(map,get_tile(map,gfx->pos_x + bound_left  - map->scrool_x, gfx->pos_y + gfx->size_y - 1 - bound_down - map->scrool_y),masq)||test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + gfx->size_y - 1 - bound_down - map->scrool_y),masq))
					gfx->pos_y-- ;
 			break ;

		case 'l':
 				while(test_tile(map,get_tile(map,gfx->pos_x + bound_left  - map->scrool_x + 1, gfx->pos_y + bound_up - map->scrool_y),masq)||test_tile(map,get_tile(map,gfx->pos_x + bound_left - map->scrool_x + 1, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
					gfx->pos_x++ ;
 			break ;

		case 'r':
 				while(test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x - 1, gfx->pos_y + bound_up - map->scrool_y),masq)||test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x - 1, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
					gfx->pos_x-- ;
 			break ;
	}

	return 0 ;
}


char move_gfx_on_map(struct gfx * gfx, struct map * map,unsigned char powa,unsigned char bound_up,unsigned char bound_down,unsigned char bound_left,unsigned char bound_right, unsigned char masq, unsigned char sens)
{
	char cpt ;
	switch (sens)
	{
		case 'u':
			for(cpt=0;cpt<powa;cpt++)
			{
 				if(! test_tile(map,get_tile(map,gfx->pos_x + bound_left  - map->scrool_x, gfx->pos_y + bound_up - map->scrool_y),masq))
				{	if(! test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + bound_up - map->scrool_y),masq))
					{	gfx->pos_y-- ;
					} else return 0 ;
				} else return 0 ;
			} ;
 			return 1 ;
 			break ;

		case 'd':
			for(cpt=0;cpt<powa;cpt++)
			{
 				if(! test_tile(map,get_tile(map,gfx->pos_x + bound_left  - map->scrool_x, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
				{	if(! test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
					{	gfx->pos_y++ ;
					} else return 0 ;
				} else return 0 ;
			} ;
			return 1 ;
 			break ;

		case 'l':
			for(cpt=0;cpt<powa;cpt++)
			{
 				if(! test_tile(map,get_tile(map,gfx->pos_x + bound_left  - map->scrool_x, gfx->pos_y + bound_up - map->scrool_y),masq))
				{	if(! test_tile(map,get_tile(map,gfx->pos_x + bound_left - map->scrool_x, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
					{	gfx->pos_x-- ;
					} else return 0 ;
				} else return 0 ;
			} ;
			return 1 ;
 			break ;

		case 'r':
			for(cpt=0;cpt<powa;cpt++)
			{
 				if(! test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + bound_up - map->scrool_y),masq))
				{	if(! test_tile(map,get_tile(map,gfx->pos_x + gfx->size_x - bound_right - map->scrool_x, gfx->pos_y + gfx->size_y - bound_down - map->scrool_y),masq))
					{	gfx->pos_x++ ;
					} else return 0 ;
				} else return 0 ;
			} ;
			return 1 ;
 			break ;
	}

	return 2 ;
}

//-----------------------------------------------------------------------

// not tested

void load_move_tab(struct gfx * gfx,short tab_move_x[100],short tab_move_y[100], unsigned char size)
{
	unsigned char cpt ;

	gfx->moving=1 ;
	
	for(cpt=0;cpt<size;cpt++)
	{
		gfx->move[cpt][0] = tab_move_x[cpt] ;
		gfx->move[cpt][1] = tab_move_y[cpt] ;
	} ;

	for(cpt=size;cpt<100;cpt++)
	{
		gfx->move[cpt][0] = 0 ;
		gfx->move[cpt][1] = 0 ;
	} ;
}


