# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import csv
import matplotlib.pyplot as plt
from tkinter import Tk, Scale, HORIZONTAL, Label
import numpy as np

def read_points_to_list(file_path):
    pairs = []
    with open(file_path, newline='') as csvfile:
        csvreader = csv.reader(csvfile)
        for row in csvreader:
            x, y = row
            pairs.append((int(x), int(y)))
    return pairs


def plot_points(points):
    x_values = [float(point[0]) for point in points]
    y_values = [float(point[1]) for point in points]

    plt.scatter(x_values, y_values)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Scatter Plot of Points')
    plt.grid(True)
    plt.show()


def plot_points_with_lines(points):
    x_values = [float(point[0]) for point in points]
    y_values = [float(point[1]) for point in points]

    plt.scatter(x_values, y_values, color='blue')

    # Connect each point to the next and to the previous
    num_points = len(points)
    for i in range(num_points):
        # Connect to the next point
        next_i = (i + 1) % num_points  # Wrap around to the first point if at the end
        plt.plot([x_values[i], x_values[next_i]], [y_values[i], y_values[next_i]], color='black')

        # Connect to the previous point
        prev_i = (i - 1 + num_points) % num_points  # Wrap around to the last point if at the beginning
        plt.plot([x_values[i], x_values[prev_i]], [y_values[i], y_values[prev_i]], color='black')

    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Points and Connecting Lines')
    plt.grid(True)
    plt.show()
# Press the green button in the gutter to run the script.

def read_csv_to_iterations(file_path):
    iterations = []
    with open(file_path, newline='') as csvfile:
        csvreader = csv.reader(csvfile)
        iteration_data = []
        for row in csvreader:
            if row[0].startswith('Iteration Best Distance'):
                if iteration_data:
                    iterations.append(iteration_data)
                iteration_data = []
                iteration_data.append(row[0])
            else:
                iteration_data.append((float(row[0]), float(row[1])))
        if iteration_data:
            iterations.append(iteration_data)
    return iterations

def plot_iteration(iteration_data, ax):
    if not iteration_data:
        return

    distance_info = iteration_data[0]
    points = iteration_data[1:]

    x_values = [point[0] for point in points]
    y_values = [point[1] for point in points]

    ax.clear()
    ax.scatter(x_values, y_values, color='blue')

    num_points = len(points)
    for i in range(num_points):
        next_i = (i + 1) % num_points
        ax.plot([x_values[i], x_values[next_i]], [y_values[i], y_values[next_i]], color='black')
        prev_i = (i - 1 + num_points) % num_points
        ax.plot([x_values[i], x_values[prev_i]], [y_values[i], y_values[prev_i]], color='black')

    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_title(f'Iteration Visualization\n{distance_info}')
    ax.grid(True)
    plt.draw()

def visualize_iterations(file_path):
    iterations = read_csv_to_iterations(file_path)
    max_iter = len(iterations) - 1

    fig, ax = plt.subplots(figsize=(8, 6))

    def update_plot(val):
        iteration = int(val)
        plot_iteration(iterations[iteration], ax)

    root = Tk()
    root.title("Iteration Visualizer")

    label = Label(root, text="Select Iteration:")
    label.pack()

    slider = Scale(root, from_=0, to=max_iter, orient=HORIZONTAL, length=400, command=update_plot)
    slider.pack()

    plot_iteration(iterations[0], ax)
    plt.ion()
    plt.show()

    root.mainloop()

def plot_csv_data(file_path):
    """
    Reads a CSV file containing a list of integers and plots them on a graph.

    Args:
    - file_path (str): Path to the CSV file.
    """
    y_values = []

    # Read the CSV file
    with open(file_path, mode='r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            if row:  # Ensure the row is not empty
                y_values.append(int(row[0]))

    # Generate x values
    x_values = list(range(len(y_values)))

    # Plot the values
    plt.figure(figsize=(10, 6))
    plt.plot(x_values, y_values, marker='o', linestyle='-', color='b', )
    plt.xlabel('X axis')
    plt.ylabel('Y axis')
    plt.title('Best Distance graph')
    plt.grid(True)

    # Display the plot
    plt.show()

if __name__ == '__main__':
    points = read_points_to_list("points.csv")
    plot_points(points)
    plot_points_with_lines(points)
    best_points = read_points_to_list("result.csv")
    plot_points_with_lines(best_points)
    plot_csv_data("best_distances.csv")
    visualize_iterations("result_path.csv")

    print("0")

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
