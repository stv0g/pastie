#ifndef RANGE_H
#define RANGE_H

template <typename T>
class Range
{
	public:
		Range(T mi, T ma) :
			min(mi), max(ma) { }

		bool contains(T v)
		{
			return (v >= min && v <= max);
		}

		T limit(T v)
		{
			if      (v > max) return max;
			else if (v < min) return min;
			else			  return v;
		}

		T min, max;
};

#endif // RANGE_H
