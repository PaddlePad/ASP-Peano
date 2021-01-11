import java.util.Arrays;
import java.util.Scanner;

public class Peano
{
	// base Kurve merken
	Direction[] pre = { Direction.UP, Direction.UP, Direction.RIGHT, Direction.DOWN, Direction.DOWN, Direction.RIGHT,
			Direction.UP, Direction.UP, };
	Direction[] curr = pre;

	public Peano()
	{
		Scanner scanner = new Scanner(System.in);

		System.out.println("Grad eingeben:");
		String gradStr = scanner.nextLine();

		int grad = Integer.parseInt(gradStr);

		Point2D[] field = peano(grad);

		Paint g = new Paint(field, grad);
	}

	public static void main(String[] args)
	{
		new Peano();
	}

	private Point2D[] peano(int grad)
	{
		int currGrad = 2;
		if (grad <= 0)
			return null;
		
		if(grad != 1)
		{
			while(currGrad <= grad)
			{
				calcNext(currGrad);
				currGrad++;
			}
		}

		Point2D[] arr = new Point2D[curr.length + 1];
		int x = 1;
		int y = 1;
		int i = 1;

		arr[0] = new Point2D(x, y);

		for (Direction d : curr)
		{
			switch (d)
			{
				case UP:
					y++;
					break;
				case DOWN:
					y--;
					break;
				case LEFT:
					x--;
					break;
				case RIGHT:
					x++;
					break;
				default:
					break;
			}

			arr[i] = new Point2D(x, y);
			i++;
		}

		return arr;
	}

	private void calcNext(int currGrad)
	{
		//System.arraycopy(curr, 0, pre, 0, curr.length);
		pre = Arrays.copyOf(curr, curr.length);
		Direction[] mir = mirror(curr);
		Direction[] rev = reverse(curr);
		Direction[] revMir = reverse(mirror(curr));

		curr = new Direction[(int) Math.pow(9, currGrad) - 1]; // für 8 Dir pro base Kurve

		int i = 0;
		int len = pre.length;
		System.arraycopy(pre, 0, curr, i, len);
		i = i + len;
		curr[i] = Direction.UP;
		i++;

		System.arraycopy(revMir, 0, curr, i, len);
		i = i + len;
		curr[i] = Direction.UP;
		i++;

		System.arraycopy(pre, 0, curr, i, len);
		i = i + len;
		curr[i] = Direction.RIGHT;
		i++;

		System.arraycopy(mir, 0, curr, i, len);
		i = i + len;
		curr[i] = Direction.DOWN;
		i++;

		System.arraycopy(rev, 0, curr, i, len);
		i = i + len;
		curr[i] = Direction.DOWN;
		i++;

		System.arraycopy(mir, 0, curr, i, len);
		i = i + len;
		curr[i] = Direction.RIGHT;
		i++;

		System.arraycopy(pre, 0, curr, i, len);
		i = i + len;
		curr[i] = Direction.UP;
		i++;

		System.arraycopy(revMir, 0, curr, i, len);
		i = i + len;
		curr[i] = Direction.UP;
		i++;

		System.arraycopy(pre, 0, curr, i, len);
	}

	private Direction[] mirror(Direction[] in) // spiegelt up&Down, gibt dircetions zurück
	{
		Direction[] out = new Direction[in.length];

		for (int i = 0; i < out.length; i++)
		{
			if (in[i] == Direction.UP)
			{
				out[i] = Direction.DOWN;
			} else if (in[i] == Direction.DOWN)
			{
				out[i] = Direction.UP;
			} else
			{
				out[i] = in[i];
			}
		}

		return out;
	}

	private Direction[] reverse(Direction[] in)
	{
		Direction[] out = new Direction[in.length];

		for (int i = 0; i < out.length; i++)
		{
			if (in[i] == Direction.UP)
			{
				out[i] = Direction.DOWN;
			} else if (in[i] == Direction.DOWN)
			{
				out[i] = Direction.UP;
			} else if (in[i] == Direction.LEFT)
			{
				out[i] = Direction.RIGHT;
			} else
			{
				out[i] = Direction.LEFT;
			}
		}

		return out;
	}
}