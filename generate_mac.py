import numpy as np

# ellipsiod parameters describing PMT
a = 0.254564663*1000
b = 0.254110205*1000
c = 0.186002389*1000
e = (a+b)/2 # xy major/minor axes averaged in actual simulation construction

d = c-165 # offset b/w z = 0 and centre of the ellipsoid describing PMT bulb

# gets the xyz coordinate of the surface of the PMT corresponding to angular coordinates
def get_surface(angles):

    # phi rotates through xy plane, theta goes from -pi/2 (-z axis) to pi/2 (+z axis)
    theta = angles[0]
    phi = angles[1]

    # x = a*np.cos(phi)*np.cos(theta)
    # y = b*np.sin(phi)*np.cos(theta)
    x = e*np.cos(phi)*np.cos(theta)
    y = e*np.sin(phi)*np.cos(theta)
    z = c*np.sin(theta) - d

    return np.array([x,y,z])

# gets the angular coordinates of an xy point on the surface of the PMT
def ptCalc(x,y):

    theta = 0
    phi = 0

    # to help python avoid dividing by 0
    if x == 0:
        if y > 0:
            phi = np.pi / 2
            # theta = np.arccos(y/b)
            theta = np.arccos(y/e)
        elif y < 0:
            phi = -np.pi / 2
            # theta = np.arccos(-y/b)
            theta = np.arccos(-y/e)
        else:
            phi = 0
            theta = np.pi/2
    else:
        # phi = np.arctan2(a*y, b*x)
        phi = np.arctan2(y,x)
        # theta = np.arccos(x/(a*np.cos(phi)))
        theta = np.arccos(x/(e*np.cos(phi)))


    return np.array([theta, phi])

# gets the normal vector of a point on the surface of the PMT described by its angular coordinates
def evaluate_norm(angles):

    theta = angles[0]
    phi = angles[1]
    
    # norm_y_nosin = 1/np.sqrt(a*(np.sin(phi))**2 + (b*np.cos(phi)/a)**2 
    #                          + (b*np.tan(theta)/c)**2)
    norm_y_nosin = 1/np.sqrt(e*(np.sin(phi))**2 + (np.cos(phi))**2 
                             + (e*np.tan(theta)/c)**2)
    norm_y = norm_y_nosin*np.sin(phi)

    # norm_z = norm_y_nosin*(b/c)*np.tan(theta)
    # norm_x = norm_y_nosin*(b/a)*np.cos(phi)
    norm_z = norm_y_nosin*(e/c)*np.tan(theta)
    norm_x = norm_y_nosin*np.cos(phi)
    
    # normalizing
    mag = np.sqrt(norm_x**2 + norm_y**2 + norm_z**2)
    norm_x /= mag
    norm_y /= mag
    norm_z /= mag

    return np.array([norm_x, norm_y, norm_z])

# gets the angular directions corresponding to a normal vector 
def get_norm_angles(norm_surface):
    
    x = norm_surface[0]
    y = norm_surface[1]
    z = norm_surface[2]

    azi = np.arctan2(y,x)

    # subtract pi/2 to account for different coordinate systems
    zen = np.arcsin(z) - np.pi/2

    return np.array([azi, zen])

# creates the macro file, sets the photon spread
def initialize_mac(filename):
    macro_filename = f"{filename}.mac"
    macro_file = open(macro_filename, "w")
    macro_file.write("/mygenerator/SetSpread 0 \n \n")
    return macro_file

# writes the generator settings for a scanpoint
def append_to_mac(macro_file, point_shoot, angles_shoot):
    L = [f"/mygenerator/SetX {point_shoot[0]} \n", 
        f"/mygenerator/SetY {point_shoot[1]} \n", 
        f"/mygenerator/SetZ {point_shoot[2]} \n", 
        f"/mygenerator/SetPAzimuthAngle {angles_shoot[0]*180/np.pi} \n",
        f"/mygenerator/SetPZenithAngle {angles_shoot[1]*180/np.pi} \n",
        f"/run/beamOn 100 \n \n"]
    macro_file.writelines(L)

# creates vertical injection simulation macro
def write_vertical_injection_mac(filename):

    macro_file = initialize_mac(filename)

    # create scanpoint arrays, can adjust depending on how fine you want the scan to be
    xs = np.linspace(-300,300,61)
    ys = np.linspace(-300,300,61)

    for i_x in range(len(xs)):
        for i_y in range(len(ys)):
            append_to_mac(macro_file, [xs[i_x], ys[i_y], 400], [0,0])

# creates xy normal injection simulation macro
def write_xy_normal_injection_mac(filename):

    macro_file = initialize_mac(filename)

    # create scanpoint arrays, can adjust depending on how fine you want the scan to be
    xs = np.linspace(-300,300,61)
    ys = np.linspace(-300,300,61)

    for i_x in range(len(xs)):
        
        x = xs[i_x]
        
        for i_y in range(len(ys)):
            
            y = ys[i_y]
            # xy_projection = (x/a)**2 + (y/b)**2
            xy_projection = x**2 + y**2

            # only want x,y that are actually on the PMT
            # if xy_projection <= 1:
            if xy_projection <= e**2:

                angular_coords = ptCalc(x,y)
                point_surface = get_surface(angular_coords)
                norm_surface = evaluate_norm(angular_coords)

                # shoot photons from this many mm away (arbitrary choice)
                step_len = 200

                angles_shoot = get_norm_angles(norm_surface)
                point_shoot = point_surface + norm_surface*step_len

                append_to_mac(macro_file, point_shoot, angles_shoot)

# creates angular normal injection simulation macro
def write_angular_normal_injection_mac(filename):
    
    macro_file = initialize_mac(filename)

    # create scanpoint arrays, can adjust depending on how fine you want the scan to be
    thetas = np.linspace(0,90,31)*np.pi/180
    phis = np.linspace(-180,180,121)*np.pi/180

    for i_t in range(len(thetas)):
        
        theta = thetas[i_t]
        
        for i_p in range(len(phis)):
            
            phi = phis[i_p]
            angular_coords = np.array([theta,phi])

            point_surface = get_surface(angular_coords)
            print("want theta =", theta*180/np.pi, "and phi =", phi*180/np.pi, "to hit", point_surface)
            norm_surface = evaluate_norm(angular_coords)

            # shoot photons from this many mm away (arbitrary choice)
            step_len = 200

            angles_shoot = get_norm_angles(norm_surface)
            point_shoot = point_surface + norm_surface*step_len
            
            append_to_mac(macro_file, point_shoot, angles_shoot)

def main(args):

    scan_options = ['vertical_injection', 'xy_normal_injection', 'angular_normal_injection']
    
    if args[0] == scan_options[0]:
        write_vertical_injection_mac(args[1])
    elif args[0] == scan_options[1]:
        write_xy_normal_injection_mac(args[1])
    elif args[0] == scan_options[2]:
        write_angular_normal_injection_mac(args[1])
    else:
        print("List of allowed scan types:", scan_options)
        return
    
if __name__=="__main__":

    import sys
    
    if len(sys.argv) < 3:
        raise ValueError("Usage: generate_mac.py <type of scan> <output file name>")

    main(sys.argv[1:])