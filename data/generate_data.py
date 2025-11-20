import math
import random
import csv

def generate_sine(filename, num_samples=1024, frequency=10.0, sample_rate=1024.0):
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        for i in range(num_samples):
            t = i / sample_rate
            val = math.sin(2 * math.pi * frequency * t)
            writer.writerow([val])
    print(f"Generated {filename}")

def generate_square(filename, num_samples=1024, frequency=10.0, sample_rate=1024.0):
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        period = sample_rate / frequency
        for i in range(num_samples):
            if (i % period) < (period / 2):
                val = 1.0
            else:
                val = -1.0
            writer.writerow([val])
    print(f"Generated {filename}")

def generate_noise(filename, num_samples=1024):
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        for i in range(num_samples):
            val = random.uniform(-1.0, 1.0)
            writer.writerow([val])
    print(f"Generated {filename}")

if __name__ == "__main__":
    generate_sine('sine.csv')
    generate_square('square.csv')
    generate_noise('noise.csv')
