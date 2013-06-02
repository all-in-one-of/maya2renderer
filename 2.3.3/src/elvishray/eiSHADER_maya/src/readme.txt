
-debug

		//debug
		if (   almost_equal(raster.x, 70.0f, 0.5f) 
			&& almost_equal(raster.y,  90.0f, 0.5f))
		{
			bool debugnow = true;
		}


		int pass_id = this->AA_seed/4 + 1;
		//debug
		if ( ( this->AA_seed == 9 )
			&& almost_equal(raster.x, 175.2f, 0.1f) 
			&& almost_equal(raster.y,  576.4f, 0.1f))
		{
			int pass_id = this->AA_seed/4 + 1;
			bool debugnow = true;
		}

		if( _isnan(out->Ci.x) || _isnan(out->Ci.y) || _isnan(out->Ci.z) )
		{
			int pass_id = this->AA_seed/4 + 1;
			int x = raster.x;
			int y = raster.y;
		}