import argparse

def parse_input(file_path):
    input_data = {}
    with open(file_path, 'r') as file:
        for line in file:
            # Extrahiere die Zeitspanne und den Wert
            parts = line.split()
            if len(parts) >= 3:
                time_span = parts[0].strip(' ns')
                value = int(parts[-1])
                input_data[int(time_span)] = value
    return input_data

def format_output(data):
    output_lines = []
    for time_span in sorted(data.keys()):
        output_lines.append(f"[{time_span} ns] {data[time_span]}")
    return output_lines

def convert_file(input_file, output_file):
    # Parse the input file
    input_data = parse_input(input_file)
    
    # Format the output data
    output_lines = format_output(input_data)
    
    # Write the output to the specified file
    with open(output_file, 'w') as file:
        for line in output_lines:
            file.write(line + '\n')

def main():
    # Set up argument parsing
    parser = argparse.ArgumentParser(description='Convert time-value data from one format to another.')
    parser.add_argument('input_file', type=str, help='Path to the input file')
    parser.add_argument('output_file', type=str, help='Path to the output file')
    
    # Parse the arguments
    args = parser.parse_args()
    
    # Convert the file
    convert_file(args.input_file, args.output_file)

if __name__ == '__main__':
    main()
