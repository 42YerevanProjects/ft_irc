
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/18 12:34:45 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/30 16:15:21 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
        throw std::runtime_error("Usage: ./ircserv <port> <password>");
    
    Server  server(argv[1], argv[2]);

    try
    {
        server.start();
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
