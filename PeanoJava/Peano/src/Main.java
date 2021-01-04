import java.util.Scanner;

public class Main {

	public static void main(String[] args) {
		Main main = new Main();
		
		Scanner scanner = new Scanner(System.in);
		System.out.println("Grad eingeben:");
		String gradStr = scanner.nextLine();

		int grad = Integer.parseInt(gradStr);
		
		Point2D[] field = main.peano(grad);
		
		Paint g = new Paint(field, grad);
	}

	private Point2D[] peano(int grad) {
		
		int len = (int) Math.pow(3, grad);
		int noFields = len * len;

		boolean up = true;
		boolean right = true;

		Point2D[] field = new Point2D[noFields];
		for (int i = 0; i < field.length; i++) {

			if (i == 0)
				field[i++] = new Point2D(1, 1);

			if (up) {
				draw(field, i++, Direction.UP);
				draw(field, i++, Direction.UP);

				if (right)
					draw(field, i++, Direction.RIGHT);
				else
					draw(field, i++, Direction.LEFT);

				draw(field, i++, Direction.DOWN);
				draw(field, i++, Direction.DOWN);

				if (right)
					draw(field, i++, Direction.RIGHT);
				else
					draw(field, i++, Direction.LEFT);

				draw(field, i++, Direction.UP);
				draw(field, i++, Direction.UP);

				if (i >= field.length)
					continue;

				// Cases
				if(field[i - 1].getY() < len)
				{
					if (field[i - 1].getX() % 3 == 0) 
						right = false;
					else
						right = true;
					
					draw(field, i, Direction.UP);
				}
				else
				{
					up = false;
					draw(field, i, Direction.RIGHT);
				}
			} 
			else 
			{
				draw(field, i++, Direction.DOWN);
				draw(field, i++, Direction.DOWN);

				if (right)
					draw(field, i++, Direction.RIGHT);
				else
					draw(field, i++, Direction.LEFT);

				draw(field, i++, Direction.UP);
				draw(field, i++, Direction.UP);

				if (right)
					draw(field, i++, Direction.RIGHT);
				else
					draw(field, i++, Direction.LEFT);

				draw(field, i++, Direction.DOWN);
				draw(field, i++, Direction.DOWN);

				if (i >= field.length)
					continue;

				// Cases
				if (field[i - 1].getY() > 1) {
					
					if (field[i - 1].getX() % 3 == 0)
						right = false;
					else
						right = true;

					draw(field, i, Direction.DOWN);
				} 
				else 
				{
					up = true;
					draw(field, i, Direction.RIGHT);
				}
			}
		}

		for (int j = 0; j < field.length; j++) {
			if (field[j] == null)
				break;

			field[j].print();
		}
		
		return field;
	}

	private void draw(Point2D[] field, int pos, Direction direction) {
		if (direction == Direction.UP)
			field[pos] = new Point2D(field[pos - 1].getX(), field[pos - 1].getY() + 1);
		else if (direction == Direction.RIGHT)
			field[pos] = new Point2D(field[pos - 1].getX() + 1, field[pos - 1].getY());
		else if (direction == Direction.DOWN)
			field[pos] = new Point2D(field[pos - 1].getX(), field[pos - 1].getY() - 1);
		else if (direction == Direction.LEFT)
			field[pos] = new Point2D(field[pos - 1].getX() - 1, field[pos - 1].getY());
	}
}

enum Direction {
	UP, DOWN, LEFT, RIGHT
}
