int clamp(int min, int val, int max) // TODO: not robust enough. Still CTD when reduced, just less often
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

